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

TCPClient::TCPClient() : _conFd(SOCK_NULL) {
};


TCPClient::~TCPClient() {
    sockDisconnect();
};

#pragma mark - Connect

bool TCPClient::sockConnect(const std::string& host,const uint16_t& port) {
    struct sockaddr sockaddr;
    
    AddressHelper::getSockaddrStruct(host, port, &sockaddr);
    if (AddressHelper::isIPv4Addr(&sockaddr)) {
        _conFd = socket(AF_INET, SOCK_STREAM, 0);
    } else if (AddressHelper::isIPv6Addr(&sockaddr)) {
        _conFd = socket(AF_INET6, SOCK_STREAM, 0);
    }
    
    if (_conFd == SOCK_NULL) {
#ifdef DEBUG
        std::cout<<"create connect socket failed!"<<std::endl;
#endif
        return false;
    }
    
    //Connect
    int result = connect(_conFd, &sockaddr, sockaddr.sa_len);
    if (SOCK_NULL == result) {
        close(_conFd);
        return false;
    }
    
    int status;
    status = fcntl(_conFd,
                   F_SETFL,
                   O_NONBLOCK);
    
    if (-1 == status) {
        close(_conFd);
        return false;
    }
    
    int reuseOn = 1;
    status = setsockopt(_conFd,
                        SOL_SOCKET,
                        SO_REUSEADDR,
                        &reuseOn,
                        sizeof(reuseOn));
    if (-1 == status) {
        close(_conFd);
        return false;
    }
    
    int nosigpipe = 1;
    status = setsockopt(_conFd,
                        SOL_SOCKET,
                        SO_NOSIGPIPE,
                        &nosigpipe,
                        sizeof(nosigpipe));
    
    if (-1 == status) {
        close(_conFd);
        return false;
    }
    
    
    //read source
    _readSource = dispatch_source_create(DISPATCH_SOURCE_TYPE_READ,
                                         _conFd,
                                         0,
                                         dispatch_get_global_queue(0, 0));
    
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
                                          _conFd,
                                          0,
                                          dispatch_get_global_queue(0, 0));
    
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
    std::cout<<"client connected! ===  listenFd:"<<_conFd<<std::endl;
    std::cout<<"host  "<<AddressHelper::getUrl(&sockaddr)<<std::endl;
    std::cout<<"*** *** *** *** ***"<<std::endl;
#endif
    return true;
}

bool TCPClient::sockDisconnect() {
    if (_conFd != SOCK_NULL) {
        close(_conFd);
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
