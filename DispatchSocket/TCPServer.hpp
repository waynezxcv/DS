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

#ifndef TCPServer_hpp
#define TCPServer_hpp

#include <vector>
#include "Socket.hpp"
#include "ConnectedClient.hpp"




namespace DispatchSocket {
    
    class TCPServer : public Socket {
    public:
        
        TCPServer();
        ~TCPServer();
        TCPServer(const TCPServer&) = delete;
        TCPServer& operator = (const TCPServer&) = delete;
        
        //开始监听
        bool sockListen();
        
        //在指定端口号监听
        bool sockListen(const uint16_t& port);
        
        //关闭服务器
        void shutdown();
        
        //读
        ssize_t sockRead(int fd, void* buffer,size_t length) override;
        
        //写
        ssize_t sockWrite(int fd, void* buffer,size_t length) override;
        
        //获取当前连接的客户端数量
        int getCurrentClientsCount() const;
        
    private:
        
        
        int _listenFd;//监听的文件描述符
        dispatch_queue_t _acceptDispatchQueue;//用于监听的Accept的dispatch队列
        dispatch_source_t _accpetSource;//在一个串行队列中执行
        std::vector<ConnectedClient *> _clients;//包含已连接的客户端文件描述符的容器
        void acceptHandler(const int& fd,const std::string& url);//accept回调
        int _addressFamily;//地址协议族
        int getListenFd () const;//获取监听的文件描述符
        bool sockClose(const int& fd);//关闭某个文件描述符
    };
}

#endif /* TCPServer_hpp */
