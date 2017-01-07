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




#include "TCPClient.hpp"

using namespace DispatchSocket;


#pragma mark - LifeCycle

TCPClient::TCPClient() : _connFd (SOCK_NULL) {
};


TCPClient::~TCPClient() {
    sockDisconnect();
};

#pragma mark - Connect

bool TCPClient::sockConnect(const std::string& host,const uint16_t& port) {
    
    struct sockaddr sockaddr;
    
    auto createSock = [](struct sockaddr* sockaddr,const std::string& host,const uint16_t& port) -> int {
        int connFd = SOCK_NULL;
        
        AddressHelper::getSockaddrStruct(host, port, sockaddr);
        if (AddressHelper::isIPv4Addr(sockaddr)) {
            connFd = socket(AF_INET, SOCK_STREAM, 0);
        } else if (AddressHelper::isIPv6Addr(sockaddr)) {
            connFd = socket(AF_INET6, SOCK_STREAM, 0);
        }
        
        
        if (connFd == SOCK_NULL) {
#ifdef DEBUG
            std::cout<<"connect fail!"<<std::endl;
#endif
            return false;
        }
        
        //Connect
        int result = connect(connFd,sockaddr, sockaddr -> sa_len);
        if (SOCK_NULL == result) {
            close(connFd);
            return SOCK_NULL;
        }
        
        //non block
        int status;
        status = fcntl(connFd,
                       F_SETFL,
                       O_NONBLOCK);
        
        if (-1 == status) {
            close(connFd);
            return SOCK_NULL;
        }
        
        //reuse address
        int reuseOn = 1;
        status = setsockopt(connFd,
                            SOL_SOCKET,
                            SO_REUSEADDR,
                            &reuseOn,
                            sizeof(reuseOn));
        if (-1 == status) {
            close(connFd);
            return SOCK_NULL;
        }
        
        //no signal pipe
        int nosigpipe = 1;
        status = setsockopt(connFd,
                            SOL_SOCKET,
                            SO_NOSIGPIPE,
                            &nosigpipe,
                            sizeof(nosigpipe));
        
        if (-1 == status) {
            close(connFd);
            return SOCK_NULL;
        }
        return connFd;
    };
    
    int connFd = createSock(&sockaddr,host,port);
    
    if (connFd == SOCK_NULL) {
#ifdef DEBUG
        std::cout<<"connect fail!"<<std::endl;
#endif
    }
    
    _connFd = connFd;
    
    //read source
    _readSource = dispatch_source_create(DISPATCH_SOURCE_TYPE_READ,
                                         _connFd,
                                         0,
                                         dispatch_get_global_queue(0, 0));
    dispatch_retain(_readSource);
    
    //read event handler
    dispatch_source_set_event_handler(_readSource, ^{
        //TODO:
    });
    
    
    //read cancel handler
    dispatch_source_set_cancel_handler(_readSource, ^{
        if (_readSource) {
            dispatch_release(_readSource);
        }
    });
    
    
    //write source
    _writeSource = dispatch_source_create(DISPATCH_SOURCE_TYPE_WRITE,
                                          _connFd,
                                          0,
                                          dispatch_get_global_queue(0, 0));
    
    dispatch_retain(_writeSource);
    
    
    //write event handler
    dispatch_source_set_event_handler(_writeSource, ^{
        //TODO:
        
    });
    
    
    //write cancel handler
    dispatch_source_set_cancel_handler(_writeSource, ^{
        if (_writeSource) {
            dispatch_release(_writeSource);
        }
    });
    
    
#ifdef DEBUG
    std::cout<<"*** *** *** *** ***"<<std::endl;
    std::cout<<"client connected! ===  listenFd:"<<_connFd<<std::endl;
    std::cout<<"host  "<<AddressHelper::getUrl(&sockaddr)<<std::endl;
    std::cout<<"*** *** *** *** ***"<<std::endl;
#endif
    
    return (SOCK_NULL != _connFd);
}

bool TCPClient::sockDisconnect() {
    
    if (_connFd != SOCK_NULL) {
        close(_connFd);
    }
    
    if (_readSource) {
        dispatch_release(_readSource);
    }
    
    if (_writeSource) {
        dispatch_release(_writeSource);
    }
    
    return true;
}

#pragma mark - I/O

ssize_t TCPClient::sockRead(int fd, void* buffer,size_t length) {
    return 0;
}

ssize_t TCPClient::sockWrite(int fd, void* buffer,size_t length) {
    return 0;
}


