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
const int kMaxConnectCount = 32;
constexpr int kMaxBufferSize = 3 * 1024;

#pragma mark - LifeCycle


TCPSocket::TCPSocket(PacketEncoder* encoder,PacketDecoder* decoder) : _encoder(encoder) ,_decoder(decoder), _sockFd(LW_SOCK_NULL) ,_addressFamily(LW_SOCK_NULL){
    _sockQueue = dispatch_queue_create("com.waynezxcv.DispatchSocket.sockQueue", DISPATCH_QUEUE_SERIAL);
    _semaphore = dispatch_semaphore_create(1);
    _connectedSockets = std::vector<TCPSocket*>();
    _flags.socketOpened = false;
    _flags.acceptSourceOpend = false;
    _flags.readSourceOpened = false;
    _flags.writeSourceOpened = false;
    _flags.readSourceSuspend = false;
    _flags.writeSourceSuspend = false;
    _flags.hasReadEOF = false;
    _flags.hasBytesAvailable = false;
    _flags.hasWriteSpaceAvailable = false;
}


TCPSocket::~TCPSocket() {
    shutdown();
    if (_flags.acceptSourceOpend) {
        dispatch_release(_accpetSource);
    }
    
    if (_flags.writeSourceOpened) {
        dispatch_release(_writeSource);
    }
    
    if (_flags.readSourceOpened) {
        dispatch_release(_readSource);
    }
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
        if (-1 == status) {
            close(sockFd);
            return LW_SOCK_NULL;
        }
        
        //reuse address
        int reuseOn = 1;
        status = setsockopt(sockFd,SOL_SOCKET,SO_REUSEADDR,&reuseOn,sizeof(reuseOn));
        if (-1 == status) {
            close(sockFd);
            return LW_SOCK_NULL;
        }
        
        //no signal pipe
        int nosigpipe = 1;
        status = setsockopt(sockFd,SOL_SOCKET,SO_NOSIGPIPE,&nosigpipe,sizeof(nosigpipe));
        
        if (-1 == status) {
            close(sockFd);
            return LW_SOCK_NULL;
        }
        
        //Bind
        status = bind(sockFd, addr, sizeof(*addr));
        if (-1 == status) {
            close(sockFd);
            return LW_SOCK_NULL;
        }
        
        //Listen
        status = listen(sockFd, kMaxConnectCount);
        if (-1 == status) {
            close(sockFd);
            return LW_SOCK_NULL;
        }
        return sockFd;
    };
    
    struct sockaddr sockaddr;
    AddressHelper::getSockaddrStruct("", port, &sockaddr);
    
    if (AddressHelper::isIPv4Addr(&sockaddr)) {
        _addressFamily = AF_INET;
        _sockFd = creatSock(AF_INET,&sockaddr);
    } else if (AddressHelper::isIPv6Addr(&sockaddr)) {
        _addressFamily = AF_INET6;
        _sockFd = creatSock(AF_INET6,&sockaddr);
    }
    if (_sockFd == LW_SOCK_NULL) {
#ifdef DEBUG
        std::cout<<"create listen socket failed!"<<std::endl;
#endif
        return false;
    }
    
    _flags.socketOpened = true;
    
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
            return ;
        }
        
        dispatch_semaphore_wait(_semaphore, DISPATCH_TIME_FOREVER);
        _flags.acceptSourceOpend = false;
        dispatch_semaphore_signal(_semaphore);
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
    
    TCPSocket* connectSocket = new TCPSocket(_encoder,_decoder);
    connectSocket->setSockFd(connFd);
    connectSocket->setAddressFamily(_addressFamily);
    
    //在新创建Sokect对象的sockeQueue中添加read/writeSource
    dispatch_async(connectSocket->getSockQueue(), ^{
        connectSocket->setupReadAndWriteSource(connFd, url);
    });
    _connectedSockets.push_back(connectSocket);
    
    if (acceptANewClientCallback != nullptr) {
        acceptANewClientCallback(url);
    }
}

#pragma mark - Connect

bool TCPSocket::sockConnect(const std::string &host, const uint16_t &port) {
    
    struct sockaddr sockaddr;
    auto createSock = [](struct sockaddr* sockaddr,const std::string& host,const uint16_t& port) -> int {
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
    _flags.socketOpened = true;
    _sockFd = connFd;
    
    if (AddressHelper::isIPv4Addr(&sockaddr)) {
        _addressFamily = AF_INET;
    } else {
        _addressFamily = AF_INET6;
    }
    
    dispatch_async(_sockQueue, ^{
        setupReadAndWriteSource(_sockFd, AddressHelper::getUrl(&sockaddr));
    });
    
    //客户端连接到主机成功通知
    if (didConnectedToHostSuccessCallBack != nullptr) {
        didConnectedToHostSuccessCallBack(host,port);
    }
    return (LW_SOCK_NULL != _sockFd);
}

bool TCPSocket::sockDisconnect() {
    int flag = sockClose(_sockFd);
    return (LW_SOCK_NULL != flag);
}

#pragma mark - close

bool TCPSocket::sockClose(const int& fd) {
    return (close(fd) != LW_SOCK_NULL);
}

void TCPSocket::shutdown() {
    sockClose(_sockFd);
    _flags.socketOpened = false;
    
    for (auto i = _connectedSockets.begin(); i != _connectedSockets.end(); ++ i) {
        TCPSocket* tcpSocket = *i;
        delete tcpSocket;
    }
    _connectedSockets.clear();
}

#pragma mark - Read / Write


void TCPSocket::setupReadAndWriteSource(const int& connFd,const std::string& url) {
    __unsafe_unretained TCPSocket* weakThis = this;
    /******************************* Read *************************************/
    _readSource = dispatch_source_create(DISPATCH_SOURCE_TYPE_READ,connFd,0,_sockQueue);
    //read source  事件处理
    
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
    
    //read source 取消处理
    dispatch_source_set_cancel_handler(_readSource, ^{
        TCPSocket* strongThis = weakThis;
        if (strongThis == nullptr) {
            return ;
        }
        dispatch_release(strongThis->_readSource);
        strongThis->_flags.readSourceOpened = false;
    });
    
    
    /******************************* Write *************************************/
    _writeSource = dispatch_source_create(DISPATCH_SOURCE_TYPE_WRITE,connFd,0,_sockQueue);
    dispatch_source_set_event_handler(_writeSource,^() {
        TCPSocket* strongThis = weakThis;
        if (strongThis == nullptr) {
            return ;
        }
        //有写空间可用
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
    //source打开标志符，用于析构函数中的release判断
    _flags.readSourceOpened = true;
    _flags.writeSourceOpened = true;
    /***********************/
    
    //这里打开read source，write source需要时才手动打开
    _sockBytesAvaliabled = 0;//可读字节置0
    resumeReadSource();
    
    _flags.hasBytesAvailable = true;//有字节可用
    _flags.writeSourceSuspend = true;//写暂停
}

void TCPSocket::readDataHandler() {
    uint8_t* buffer = (uint8_t *)malloc(kMaxBufferSize);
    ssize_t results = read(_sockFd, buffer, kMaxBufferSize);
    
    if (results < 0) {
        printf("error\n");
    } else if(results == 0) {
        readEOFHandler();
        _sockBytesAvaliabled = 0;
    } else {
        ssize_t len = results;
        _decoder->decode(buffer,len);
    }
    free(buffer);
}

void TCPSocket::writeData(uint8_t* buffer,ssize_t len) {
    resumeWriteSource();
    
    
}

void TCPSocket::writeDataHanlder() {
    uint8_t* buffer = (uint8_t *)malloc(kMaxBufferSize);
    ssize_t results = write(_sockFd, buffer, kMaxBufferSize);
    if (results < 0) {
        printf("error\n");
    } else if(results == 0) {
        readEOFHandler();
        _sockBytesAvaliabled = 0;
    } else {
        ssize_t len = results;
        _encoder->encode(buffer,len);
    }
}

//对方断开了连接
void TCPSocket::readEOFHandler() {
    _flags.hasReadEOF = true;
    suspendReadSource();
    printf("do read eof\n");
}

void TCPSocket::suspendReadSource() {
    if (_flags.readSourceSuspend) {
        return;
    }
    dispatch_suspend(_readSource);
    _flags.readSourceSuspend = true;
    printf("suspend read source\n");
}

void TCPSocket::suspendWriteSource() {
    if (_flags.writeSourceSuspend) {
        return;
    }
    dispatch_suspend(_writeSource);
    _flags.writeSourceSuspend = true;
    printf("suspend write source\n");
}

void TCPSocket::resumeReadSource() {
    dispatch_resume(_readSource);
    _flags.readSourceSuspend = false;
    printf("resume read source\n");
}

void TCPSocket::resumeWriteSource() {
    dispatch_resume(_writeSource);
    _flags.writeSourceSuspend = false;
    printf("resume write source\n");
}

#pragma mark - Setter & Getter

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

#pragma mark - Others

unsigned TCPSocket::currentConnectedSocketsCount() const {
    return static_cast<unsigned>(_connectedSockets.size());
}

