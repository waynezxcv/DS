/*
 Copyright (c) 2016 waynezxcv <liuweiself@126.com>
 
 https://github.com/waynezxcv/ELoop
 
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

TCPServer::TCPServer() : _listenFd(SOCK_NULL) {
    _clientFds = std::vector<int>();
    _acceptDispatchQueue = dispatch_queue_create("com.waynezxcv.DispatchSocket.acceptQueue", DISPATCH_QUEUE_SERIAL);//串行队列
}


TCPServer::~TCPServer() {
    if (_listenFd != SOCK_NULL) {
        close(_listenFd);
    }
    if (_acceptDispatchQueue) {
        dispatch_release(_accpetSource);
    }
}


#pragma mark - Listen & Accept

bool TCPServer::sockListen() {
    return sockListen(0);
}

bool TCPServer::sockListen(const uint16_t &port) {
    
    //创建socket匿名函数
    auto creatSock = [](int domain,const struct sockaddr* addr) -> int {
        
        //Get socket file descriptor
        int sockFd = socket(domain, SOCK_STREAM, 0);
        
        if (SOCK_NULL == sockFd) {
            return SOCK_NULL;
        }
        
        int status;
        
        status = fcntl(sockFd,
                       F_SETFL,
                       O_NONBLOCK);
        if (-1 == status) {
            close(sockFd);
            return SOCK_NULL;
        }
        
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
        _listenFd = creatSock(AF_INET,&sockaddr);
    } else if (AddressHelper::isIPv6Addr(&sockaddr)) {
        _listenFd = creatSock(AF_INET,&sockaddr);
    }
    
    if (_listenFd == SOCK_NULL) {
#ifdef DEBUG
        std::cout<<"create listen socket failed!"<<std::endl;
#endif
        return false;
    }
    
#ifdef DEBUG
    uint16_t p;
    std::string i;
    sockGetSockName(_listenFd,i, p);
    std::cout<<"server start listen!  listenFd:"<<_listenFd<<std::endl;
    std::cout<<"host:"<<sockGetIfaddrs()<<std::endl;
    std::cout<<"port:"<<p<<std::endl;
#endif
    //创建accpet source，类型Read
    _accpetSource = dispatch_source_create(DISPATCH_SOURCE_TYPE_READ,
                                           _listenFd,
                                           0,
                                           _acceptDispatchQueue);
    
    __unsafe_unretained TCPServer* weakThis = this;
    
    //指定发生READ事件时执行的回调
    dispatch_source_set_event_handler(_accpetSource, ^{
        
        TCPServer* strongThis = weakThis;
        if (strongThis == nullptr) {
            return;
        }
        
        unsigned long i = 0;
        unsigned long pending = dispatch_source_get_data(_accpetSource);
        
        printf("pending :%lu\n",pending);
        
        
        if ((++i < pending)) {
            //do accept;
            weakThis->acceptHandler(_listenFd);
        }
    });
    
    //指定取消READ事件时执行的回调
    dispatch_source_set_cancel_handler(_accpetSource, ^{
        printf("cancel");
        if (_accpetSource) {
            dispatch_release(_accpetSource);//释放内存
        }
    });
    
    //启动dispatch source
    dispatch_resume(_accpetSource);
    printf("resume!\n");
    return (SOCK_NULL != _listenFd);
}

void TCPServer::acceptHandler(const int& fd) {
    printf("accept!");
    
    
    
}


#pragma mark - Close

bool TCPServer::sockClose(const int& fd) {
    return close(fd);
}

#pragma mark - I/O

ssize_t TCPServer::sockRead(int fd, void* buffer,size_t length) {
    return 0;
}

ssize_t TCPServer::sockWrite(int fd, void* buffer,size_t length) {
    return 0;
    
}

int TCPServer::getListenFd() const {
    return _listenFd;
}

