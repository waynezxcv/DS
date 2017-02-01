/*
 Copyright (c) 2016 waynezxcv <liuweiself@126.com>
 
 https://github.com/waynezxcv/DispatchSocket
 
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */

#include "TCPSocket.hpp"

using namespace DispatchSocket;
constexpr int kMaxBufferSize = 3 * 1024;
const int kMaxConnectCount = 32;

#pragma mark - LifeCycle

TCPSocket::TCPSocket() : _sockFd(LW_SOCK_NULL) ,_addressFamily(LW_SOCK_NULL){
    _sockQueue = dispatch_queue_create("com.waynezxcv.DispatchSocket.sockQueue", DISPATCH_QUEUE_SERIAL);
    _connectedSockets = std::map<std::string, std::shared_ptr<TCPSocket>> ();
    _semaphore = dispatch_semaphore_create(1);
    _flags.socketOpened = false;
    _flags.acceptSourceOpend = false;
    _flags.readSourceOpened = false;
    _flags.writeSourceOpened = false;
    _flags.readSourceSuspend = false;
    _flags.writeSourceSuspend = false;
    _flags.hasReadEOF = false;
    _flags.hasBytesAvailable = false;
    _flags.hasWriteSpaceAvailable = false;
    _writeQueue = std::queue<std::shared_ptr<WritePacket>>();
    _currentRead = std::make_shared<Data>();
}

TCPSocket::~TCPSocket() {
    if (_flags.acceptSourceOpend == true) {
        dispatch_release(_accpetSource);
        _flags.acceptSourceOpend = false;
    }
    if (_flags.readSourceOpened == true && _flags.readSourceSuspend == false) {
        dispatch_release(_readSource);
        _flags.readSourceOpened = false;
    }
    if (_flags.writeSourceOpened == true && _flags.writeSourceSuspend == false) {
        dispatch_release(_writeSource);
        _flags.writeSourceOpened = false;
    }
    shutdown();
    dispatch_release(_sockQueue);
    dispatch_release(_semaphore);
}


#pragma mark - Listen

bool TCPSocket::sockListen() {
    return TCPSocket::sockListen(0);
}

bool TCPSocket::sockListen(const uint16_t &port) {
    
    auto creatSock = [](int domain,const struct sockaddr* addr) -> int {
        //Get socket file descriptor
        int sockFd = socket(domain, SOCK_STREAM, 0);
        if (LW_SOCK_NULL == sockFd) {
            return LW_SOCK_NULL;
        }
        int status;
        //non block
        status = fcntl(sockFd,F_SETFL,O_NONBLOCK);
        if (LW_SOCK_NULL == status) {
            close(sockFd);
            return LW_SOCK_NULL;
        }
        
        //reuse address
        int reuseOn = 1;
        status = setsockopt(sockFd,SOL_SOCKET,SO_REUSEADDR,&reuseOn,sizeof(reuseOn));
        if (LW_SOCK_NULL == status) {
            close(sockFd);
            return LW_SOCK_NULL;
        }
        
        //no signal pipe
        int nosigpipe = 1;
        status = setsockopt(sockFd,SOL_SOCKET,SO_NOSIGPIPE,&nosigpipe,sizeof(nosigpipe));
        
        if (LW_SOCK_NULL == status) {
            close(sockFd);
            return LW_SOCK_NULL;
        }
        
        //Bind
        status = bind(sockFd, addr, sizeof(*addr));
        if (LW_SOCK_NULL == status) {
            close(sockFd);
            return LW_SOCK_NULL;
        }
        
        //Listen
        status = listen(sockFd, kMaxConnectCount);
        if (LW_SOCK_NULL == status) {
            close(sockFd);
            return LW_SOCK_NULL;
        }
        return sockFd;
    };
    
    int sockFd = LW_SOCK_NULL;
    struct sockaddr sockaddr;
    AddressHelper::getSockaddrStruct("", port, &sockaddr);
    
    if (AddressHelper::isIPv4Addr(&sockaddr)) {
        _addressFamily = AF_INET;
        sockFd = creatSock(AF_INET,&sockaddr);
    } else if (AddressHelper::isIPv6Addr(&sockaddr)) {
        _addressFamily = AF_INET6;
        sockFd = creatSock(AF_INET6,&sockaddr);
    }
    if (sockFd == LW_SOCK_NULL) {
#ifdef DEBUG
        std::cout<<"create listen socket failed!"<<std::endl;
#endif
        return false;
    }
    
    _flags.socketOpened = true;
    _sockFd = sockFd;
    _role = TCPSocketRoleServer;
    
    std::string host;
    uint16_t p;
    sockGetSockName(_sockFd,host, p);
    if (startListenCallBack != nullptr) {
        //notitication....
        startListenCallBack(sockGetIfaddrs(),p);
    }
    
    //accept source
    __unsafe_unretained TCPSocket* weakThis = this;
    _accpetSource = dispatch_source_create(DISPATCH_SOURCE_TYPE_READ,_sockFd,0,_sockQueue);
    
    //event handler
    dispatch_source_set_event_handler(_accpetSource, ^{
        TCPSocket* strongThis = weakThis;
        if (strongThis == nullptr) {
            return;
        }
        unsigned long pending = dispatch_source_get_data(_accpetSource);
        if (pending > 0 && _connectedSockets.size() < kMaxConnectCount ) {
            struct sockaddr c_addr;
            
            AddressHelper::getSockaddrStruct(sockGetIfaddrs(), p, &c_addr);
            weakThis->acceptHandler(_sockFd,AddressHelper::getUrl(&c_addr));
        }
    });
    
    dispatch_source_set_cancel_handler(_accpetSource, ^{
        TCPSocket* strongThis = weakThis;
        if (strongThis == nullptr) {
            return;
        }
        _flags.acceptSourceOpend = false;
        dispatch_release(_accpetSource);
    });
    
    dispatch_resume(_accpetSource);
    _flags.acceptSourceOpend = true;
    return (LW_SOCK_NULL != _sockFd);
}

#pragma mark - Accept

void TCPSocket::acceptHandler(const int& fd,const std::string& url) {
    struct sockaddr* sockaddr = nullptr;
    socklen_t addrLen;
    if (_addressFamily == AF_INET) {
        struct sockaddr_in addr_in;
        addrLen = sizeof(addr_in);
        sockaddr = (struct sockaddr *)&addr_in;
        
    } else if(_addressFamily == AF_INET6) {
        
        struct sockaddr_in6 addr_in6;
        addrLen = sizeof(addr_in6);
        sockaddr = (struct sockaddr *)&addr_in6;
    }
    
    int connFd = accept(fd,sockaddr, &addrLen);
    if (connFd == LW_SOCK_NULL) {
#ifdef DEBUG
        printf("accept fail!\n");
#endif
        return;
    }
    
    //get peer addr
    struct sockaddr peerAddr;
    std::string peerIp;
    uint16_t peerPort;
    sockGetPeerName(connFd, peerIp, peerPort);
    AddressHelper::getSockaddrStruct(peerIp, peerPort, &peerAddr);
    
    //set
    std::shared_ptr<TCPSocket> connectSocket = std::make_shared<TCPSocket>();
    connectSocket->setSockFd(connFd);
    connectSocket->setAddressFamily(_addressFamily);
    connectSocket->_role = TCPSocketRoleClient;
    connectSocket->_URL = AddressHelper::getUrl(&peerAddr);
    connectSocket->_parentSocket = this;
    
    //在新创建Sokect对象的sockeQueue中添加read/writeSource
    dispatch_async(connectSocket->getSockQueue(), ^{
        connectSocket->setupReadAndWriteSource(connFd, connectSocket->_URL);
    });
    
    _connectedSockets.insert(std::pair<std::string,std::shared_ptr<TCPSocket>>(connectSocket->_URL,connectSocket));
    if (acceptANewClientCallback != nullptr) {
        acceptANewClientCallback(connectSocket->_URL);
    }
}

#pragma mark - Connect

bool TCPSocket::sockConnect(const std::string &host, const uint16_t &port) {
    struct sockaddr sockaddr;
    
    auto createSock = [this](struct sockaddr* sockaddr,const std::string& host,const uint16_t& port) -> int {
        
        int connFd = LW_SOCK_NULL;
        AddressHelper::getSockaddrStruct(host, port, sockaddr);
        if (AddressHelper::isIPv4Addr(sockaddr)) {
            connFd = socket(AF_INET, SOCK_STREAM, 0);
        } else if (AddressHelper::isIPv6Addr(sockaddr)) {
            connFd = socket(AF_INET6, SOCK_STREAM, 0);
        }
        if (connFd == LW_SOCK_NULL) {
#ifdef DEBUG
            std::cout<<"connect fail!"<<std::endl;
#endif
            return false;
        }
        
        //Connect
        int result = connect(connFd,sockaddr, sockaddr -> sa_len);
        if (LW_SOCK_NULL == result) {
            close(connFd);
            return LW_SOCK_NULL;
        }
        
        //non block
        int status;
        status = fcntl(connFd,F_SETFL,O_NONBLOCK);
        if (LW_SOCK_NULL == status) {
            close(connFd);
            return LW_SOCK_NULL;
        }
        
        //reuse address
        int reuseOn = 1;
        status = setsockopt(connFd,SOL_SOCKET,SO_REUSEADDR,&reuseOn,sizeof(reuseOn));
        if (LW_SOCK_NULL == status) {
            close(connFd);
            return LW_SOCK_NULL;
        }
        
        //no signal pipe
        int nosigpipe = 1;
        status = setsockopt(connFd,SOL_SOCKET,SO_NOSIGPIPE,&nosigpipe,sizeof(nosigpipe));
        if (LW_SOCK_NULL == status) {
            close(connFd);
            return LW_SOCK_NULL;
        }
        return connFd;
    };
    int connFd = createSock(&sockaddr,host,port);
    
    if (connFd == LW_SOCK_NULL) {
#ifdef DEBUG
        std::cout<<"connect fail!\n"<<std::endl;
        return false;
#endif
    }
    
    //set
    _flags.socketOpened = true;
    
    
    _role = TCPSocketRoleClient;
    _sockFd = connFd;
    
    if (AddressHelper::isIPv4Addr(&sockaddr)) {
        _addressFamily = AF_INET;
    } else {
        _addressFamily = AF_INET6;
    }
    _URL = AddressHelper::getUrl(&sockaddr);
    
    if (didConnectedToHostSuccessCallBack != nullptr) {
        didConnectedToHostSuccessCallBack(host,port);
    }
    
    dispatch_async(_sockQueue, ^{
        setupReadAndWriteSource(_sockFd, AddressHelper::getUrl(&sockaddr));
    });
    
    return (LW_SOCK_NULL != _sockFd);
}

bool TCPSocket::sockDisconnect() {
    int flag = sockClose(_sockFd);
    return (LW_SOCK_NULL != flag);
}

bool TCPSocket::sockDisconnect(const std::string& url) {
    
    
    std::map<std::string, std::shared_ptr<TCPSocket>>::iterator itr = _connectedSockets.find(url);
    std::shared_ptr<TCPSocket> tcpSocket = itr->second;
    sockClose(tcpSocket->_sockFd);
    _connectedSockets.erase(itr);
    return true;
}

#pragma mark - close

bool TCPSocket::sockClose(const int& fd) {
    return (close(fd) != LW_SOCK_NULL);
}

void TCPSocket::shutdown() {
    if (_role == TCPSocketRoleServer) {
        _connectedSockets.clear();
    }
    sockClose(_sockFd);
    _flags.socketOpened = false;
}

#pragma mark - Read / Write Source

void TCPSocket::suspendReadSource() {
    if (_flags.readSourceOpened == false || _flags.readSourceSuspend) {
        return;
    }
    dispatch_suspend(_readSource);
    _flags.readSourceSuspend = true;
}

void TCPSocket::suspendWriteSource() {
    if (_flags.writeSourceOpened == false || _flags.writeSourceSuspend) {
        return;
    }
    
    dispatch_suspend(_writeSource);
    _flags.writeSourceSuspend = true;
}

void TCPSocket::resumeReadSource() {
    if (_flags.readSourceOpened == false) {
        return;
    }
    
    dispatch_resume(_readSource);
    _flags.readSourceSuspend = false;
}

void TCPSocket::resumeWriteSource() {
    if (_flags.writeSourceOpened == false || _flags.writeSourceSuspend != true) {
        return;
    }
    dispatch_resume(_writeSource);
    _flags.writeSourceSuspend = false;
    printf("resume write source\n");
}

void TCPSocket::setupReadAndWriteSource(const int& connFd,const std::string& url) {
    __unsafe_unretained TCPSocket* weakThis = this;
    _readSource = dispatch_source_create(DISPATCH_SOURCE_TYPE_READ,connFd,0,_sockQueue);
    dispatch_source_set_event_handler(_readSource, ^{
        TCPSocket* strongThis = weakThis;
        if (strongThis == nullptr) {
            return ;
        }
        strongThis->_sockBytesAvaliabled = dispatch_source_get_data(strongThis->_readSource);
        
        //readSource有字节可读
        if (strongThis -> _sockBytesAvaliabled > 0) {
            strongThis -> readDataHandler();
        } else {//对方断开了连接
            strongThis->readEOFHandler();
        }
    });
    
    dispatch_source_set_cancel_handler(_readSource, ^{
        TCPSocket* strongThis = weakThis;
        if (strongThis == nullptr) {
            return ;
        }
        dispatch_release(strongThis->_readSource);
        strongThis->_flags.readSourceOpened = false;
    });
    
    
    _writeSource = dispatch_source_create(DISPATCH_SOURCE_TYPE_WRITE,connFd,0,_sockQueue);
    dispatch_source_set_event_handler(_writeSource,^() {
        TCPSocket* strongThis = weakThis;
        if (strongThis == nullptr) {
            return ;
        }
        
        strongThis->_flags.hasWriteSpaceAvailable = true;
        strongThis -> writeDataHanlder();
    });
    
    dispatch_source_set_cancel_handler(_writeSource,^{
        TCPSocket* strongThis = weakThis;
        if (strongThis == nullptr) {
            return ;
        }
        dispatch_release(strongThis->_writeSource);
        strongThis->_flags.writeSourceOpened = false;
    });
    
    /***********************/
    _flags.readSourceOpened = true;
    _flags.writeSourceOpened = true;
    /***********************/
    
    //这里打开read source，write source需要时才手动打开
    _sockBytesAvaliabled = 0;
    resumeReadSource();
    
    _flags.hasBytesAvailable = true;
    _flags.writeSourceSuspend = true;
}


void TCPSocket::writeData(const Data& data,int type) {
    std::shared_ptr<WritePacket> writePacket = std::make_shared<WritePacket>(data,type);
    _writeQueue.push(writePacket);//加入队列
    resumeWriteSource();
}


void TCPSocket::writeDataHanlder() {
    if (_writeQueue.size() == 0) {
        suspendWriteSource();
        return;
    }
    
    printf("write handler\n");
    
    _currentWrite = _writeQueue.front();
    _writeQueue.pop();
    
    int totalLen = 0;
    
    if (_currentWrite->length <= kMaxBufferSize) {
        
        uint8_t* buffer = (uint8_t *)malloc(sizeof(uint8_t) * _currentWrite->length);
        while (totalLen < _currentWrite -> length) {
            ssize_t results = write(_sockFd,buffer,_currentWrite -> length);
            
            if (results < 0) {
                
                sockDisconnect();
                break;
                
            } else if(results == 0) {
                
                break;
                
            } else {
                
                ssize_t len = results;
                totalLen += len;
            }
        }
        
        printf("total len :%d\n",totalLen);
    } else {
        
    }
}

void TCPSocket::readDataHandler() {
    
    uint8_t buffer[kMaxBufferSize];
    ssize_t results = read(_sockFd, buffer, kMaxBufferSize);
    
    if (results < 0) {
        
        sockClose(_sockFd);
        
    } else if(results == 0) {
        
        readEOFHandler();
        _sockBytesAvaliabled = 0;
        
    } else {
        
        dispatch_semaphore_wait(_semaphore, DISPATCH_TIME_FOREVER);
        
        ssize_t len = results;
        Data data = Data(buffer, len);
        _currentRead->appendData(data);
        
        if (_currentRead->length() >= sizeof(unsigned long)/*length*/ + sizeof(int)/*type*/) {
            Data headerData  = _currentRead->subData(0,sizeof(unsigned long) + sizeof(int));
            
                    
            PacketHeader header = PacketHeader(headerData);
            
            /*
             printf("current read len:%zd\n",_currentRead->length());
             printf("header len:%zd\n",header.length);
             */
            
            if (_currentRead -> length() == header.length) {
                
                Data p = _currentRead->subData(sizeof(unsigned long) + sizeof(int), header.length - (sizeof(unsigned long) + sizeof(int)));
                std::shared_ptr<Data> payload = std::make_shared<Data>(p);
                int type = header.type;
                
                if (didReceivedDataCallBack != nullptr) {
                    didReceivedDataCallBack(payload,type);
                }
                
                _currentRead->empty();
            }
        }
        
        dispatch_semaphore_signal(_semaphore);
    }
}


void TCPSocket::readEOFHandler() {
    _flags.hasReadEOF = true;
    suspendReadSource();
    //从parentSocket的map中删除
    std::string key = _URL;
    if (this->_parentSocket != nullptr) {
        TCPSocket* parent = this->_parentSocket;
        parent -> sockDisconnect(key);
    }
}

#pragma mark - Setter & Getter

//通过URL获取TCPSocket实例
std::shared_ptr<TCPSocket> TCPSocket::tcpsocketForURL(const std::string& URL) {
    if (_role == TCPSocketRoleServer) {
        std::map<std::string,std::shared_ptr<TCPSocket>>::iterator iterator = _connectedSockets.find(URL);
        std::shared_ptr<TCPSocket> tcpSocket = iterator->second;
        return tcpSocket;
    } else {
        return nullptr;
    }
}

int TCPSocket::getSockFd() const {
    return _sockFd;
}

void TCPSocket::setSockFd (const int& fd) {
    _sockFd = fd;
}

void TCPSocket::setAddressFamily(const int& af) {
    _addressFamily = af;
}

int TCPSocket::getSockAddressFamily() const {
    return _addressFamily;
}

dispatch_queue_t TCPSocket::getSockQueue() const {
    return _sockQueue;
}

TCPSocketRole TCPSocket::role () const {
    return _role;
}

#pragma mark - Others

unsigned TCPSocket::currentConnectedSocketsCount() const {
    return static_cast<unsigned>(_connectedSockets.size());
}

