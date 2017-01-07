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




#ifndef TCPClient_hpp
#define TCPClient_hpp

#include "Socket.hpp"

namespace DispatchSocket {
    class TCPClient : public Socket {
    public:
        //构造函数
        TCPClient();
        
        //析构函数
        ~TCPClient();
        
        //拷贝构造函数
        TCPClient(const TCPClient&) = delete;
        
        //拷贝赋值运算符
        TCPClient& operator = (const TCPClient&) = delete;
        
        //通过主机地址和端口号链接
        bool sockConnect(const std::string& host,const uint16_t& port);
        
        //断开连接
        bool sockDisconnect();
        
        //读
        ssize_t sockRead(int fd, void* buffer,size_t length) override;
        
        //写
        ssize_t sockWrite(int fd, void* buffer,size_t length) override;
        
    private:
        int _conFd;
    };
}


#endif /* TCPClient_hpp */