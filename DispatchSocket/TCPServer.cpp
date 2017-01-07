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




#include "TCPServer.hpp"


#define MAX_CONNECT_COUNT 32

using namespace DispatchSocket;

#pragma mark - LifeCycle

TCPServer::TCPServer() : _listenFd(SOCK_NULL),_addressFamily(SOCK_NULL) {
    _clients = std::vector<ConnectedClient *>();
    _acceptDispatchQueue = dispatch_queue_create("com.waynezxcv.DispatchSocket.acceptQueue", DISPATCH_QUEUE_SERIAL);//串行队列
}


TCPServer::~TCPServer() {
    if (_acceptDispatchQueue) {
        dispatch_release(_acceptDispatchQueue);
    }
    shutdown();
}


#pragma mark - Listen & Accept

bool TCPServer::sockListen() {
    return sockListen(0);
}

bool TCPServer::sockListen(const uint16_t &port) {
    auto creatSock = [](int domain,const struct sockaddr* addr) -> int {
        
        //Get socket file descriptor
        int sockFd = socket(domain, SOCK_STREAM, 0);
        
        if (SOCK_NULL == sockFd) {
            return SOCK_NULL;
        }
        
        int status;
        
        //non block
        status = fcntl(sockFd,
                       F_SETFL,
                       O_NONBLOCK);
        if (-1 == status) {
            close(sockFd);
            return SOCK_NULL;
        }
        
        //reuse address
        int reuseOn = 1;
        status = setsockopt(sockFd,
                            SOL_SOCKET,
                            SO_REUSEADDR,
                            &reuseOn,
                            sizeof(reuseOn));
        if (-1 == status) {
            close(sockFd);
            return SOCK_NULL;
        }
        
        //no signal pipe
        int nosigpipe = 1;
        status = setsockopt(sockFd,
                            SOL_SOCKET,
                            SO_NOSIGPIPE,
                            &nosigpipe,
                            sizeof(nosigpipe));
        
        if (-1 == status) {
            close(sockFd);
            return SOCK_NULL;
        }
        
        //Bind
        status = bind(sockFd, addr, sizeof(*addr));
        if (-1 == status) {
            close(sockFd);
            return SOCK_NULL;
        }
        
        //Listen
        status = listen(sockFd, MAX_CONNECT_COUNT);
        if (-1 == status) {
            close(sockFd);
            return SOCK_NULL;
        }
        return sockFd;
    };
    
    struct sockaddr sockaddr;
    AddressHelper::getSockaddrStruct("", port, &sockaddr);
    
    if (AddressHelper::isIPv4Addr(&sockaddr)) {
        _addressFamily = AF_INET;
        _listenFd = creatSock(AF_INET,&sockaddr);
    } else if (AddressHelper::isIPv6Addr(&sockaddr)) {
        _addressFamily = AF_INET6;
        _listenFd = creatSock(AF_INET6,&sockaddr);
    }
    
    if (_listenFd == SOCK_NULL) {
#ifdef DEBUG
        std::cout<<"create listen socket failed!"<<std::endl;
#endif
        return false;
    }
    
#ifdef DEBUG
    uint16_t p;
    std::string host;
    sockGetSockName(_listenFd,host, p);
    std::cout<<"server start listen!  listenFd:"<<_listenFd<<std::endl;
    std::cout<<"host:"<<sockGetIfaddrs()<<std::endl;
    std::cout<<"port:"<<p<<std::endl;
#endif
    
    
    //accept source
    _accpetSource = dispatch_source_create(DISPATCH_SOURCE_TYPE_READ,
                                           _listenFd,
                                           0,
                                           _acceptDispatchQueue);
    dispatch_retain(_accpetSource);
    
    __unsafe_unretained TCPServer* weakThis = this;
    
    dispatch_source_set_event_handler(_accpetSource, ^{
        
        TCPServer* strongThis = weakThis;
        if (strongThis == nullptr) {
            return;
        }
        
        unsigned long pending = dispatch_source_get_data(_accpetSource);
        if (pending > 0 && _clients.size() < MAX_CONNECT_COUNT) {
            struct sockaddr c_addr;
            AddressHelper::getSockaddrStruct(sockGetIfaddrs(), p, &c_addr);
            weakThis->acceptHandler(_listenFd,AddressHelper::getUrl(&c_addr));
        }
    });
    
    dispatch_source_set_cancel_handler(_accpetSource, ^{
#ifdef DEBUG
        printf("cancel accept source\n");
#endif
        if (_accpetSource) {
            dispatch_release(_accpetSource);
        }
    });
    
    dispatch_resume(_accpetSource);
    
    return (SOCK_NULL != _listenFd);
}



void TCPServer::acceptHandler(const int& fd,const std::string& url) {
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
    if (connFd == SOCK_NULL) {
#ifdef DEBUG
        printf("accept fail!\n");
#endif
        return;
    }
    
    //write
    dispatch_source_t writeSource = dispatch_source_create(DISPATCH_SOURCE_TYPE_WRITE,
                                                           connFd,
                                                           0,
                                                           dispatch_get_global_queue(0, 0));
    
    dispatch_source_set_event_handler(writeSource, ^{
        //TODO:
        printf("write..");
    });
    
    
    dispatch_source_set_cancel_handler(writeSource, ^{
#ifdef DEBUG
        printf("cancel write source\n");
#endif
        
        
    });
    
    
    //read
    dispatch_source_t readSource = dispatch_source_create(DISPATCH_SOURCE_TYPE_READ,
                                                          connFd,
                                                          0,
                                                          dispatch_get_global_queue(0, 0));
    
    
    dispatch_source_set_event_handler(readSource, ^{
        printf("read...");
        
        //        void* buffer;
        //        size_t available = dispatch_source_get_data(readSource);
        //        int length = read(connFd, buffer, available);
        
        
        
    });
    
    
    dispatch_source_set_cancel_handler(readSource, ^{
        
#ifdef DEBUG
        printf("cancel read source\n");
#endif
        
    });
    
    ConnectedClient* client = new ConnectedClient(connFd,url);
    client->setReadSource(readSource);
    client->setWriteSource(writeSource);
    _clients.push_back(client);
    
    //获取对端ip地址和端口号
#ifdef DEBUG
    std::string clientAddr = AddressHelper::getUrl(sockaddr);
    std::cout<<"*** *** *** *** ***"<<std::endl;
    std::cout<<"accept!  connect Fd:"<<connFd<<std::endl;
    std::cout<<"client address:"<<clientAddr<<std::endl;
    std::cout<<"current connects count:"<<getCurrentClientsCount()<<std::endl;
    std::cout<<"*** *** *** *** ***"<<std::endl;
#endif
    
}

#pragma mark - I/O

ssize_t TCPServer::sockRead(int fd, void* buffer,size_t length) {
    return 0;
}

ssize_t TCPServer::sockWrite(int fd, void* buffer,size_t length) {
    return 0;
}

#pragma mark - Close

void TCPServer::shutdown() {
    for (auto i = _clients.begin(); i != _clients.end(); ++ i) {
        ConnectedClient* client = *i;
        sockClose(client->getFd());
        delete client;
    }
    
    _clients.clear();
    
    if (_listenFd != SOCK_NULL) {
        sockClose(_listenFd);
    }
    
    if (_accpetSource) {
        dispatch_release(_accpetSource);
    }
}

bool TCPServer::sockClose(const int& fd) {
    return close(fd);
}

#pragma mark - Getter

int TCPServer::getListenFd() const {
    return _listenFd;
}

int TCPServer::getCurrentClientsCount() const {
    return static_cast<int>(_clients.size());
}


