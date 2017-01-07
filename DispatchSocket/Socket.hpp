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



#ifndef Socket_hpp
#define Socket_hpp

#include "AddressHelper.hpp"
#include <dispatch/dispatch.h>



#define SOCK_NULL -1
#define BUFFER_SIZE 32768


namespace DispatchSocket {
    
    
    class Socket {
    public:
        Socket(){};
        virtual ~Socket(){};
        
        //读
        virtual ssize_t sockRead(int fd, void* buffer,size_t length) = 0;
        
        //写
        virtual ssize_t sockWrite(int fd, void* buffer,size_t length) = 0;
        
        //获取socket
        void sockGetSockName(const int& fd, std::string &ip,uint16_t &port) const;
        
        //获取对端socket
        void sockGetPeerName(const int& fd, std::string &ip,uint16_t &port) const;
        
        //获取局域网IP
        std::string sockGetIfaddrs() const;
    };
}


#endif /* Socket_hpp */
