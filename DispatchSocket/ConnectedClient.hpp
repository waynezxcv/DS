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




#ifndef ConnectedClient_hpp
#define ConnectedClient_hpp

#include <iostream>
#include <dispatch/dispatch.h>


namespace DispatchSocket {
    
    //在服务端对已连接的客户端的抽象模型
    class ConnectedClient {
        
        friend class TCPServer;
        
        explicit ConnectedClient(const int&fd ,const std::string& url) : _connectFd(fd),_url(url) {};
        ~ConnectedClient();
        ConnectedClient(const ConnectedClient&) = delete;
        ConnectedClient& operator = (const ConnectedClient&) = delete;
        
        void setReadSource(const dispatch_source_t& source);
        void setWriteSource(const dispatch_source_t& source);
        
        dispatch_source_t getReadSource() const;
        dispatch_source_t getWriteSource() const;
        
        int getFd() const;
        std::string getURL() const;
        
        std::string _url;
        int _connectFd;
        dispatch_source_t _readSource;
        dispatch_source_t _writeSource;
    };
}


#endif /* ConnectedClient_hpp */