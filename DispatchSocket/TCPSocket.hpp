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

#ifndef TCPSocket_hpp
#define TCPSocket_hpp

#include "Socket.hpp"
#include "SocketDataPacket.hpp"
#include <vector>



namespace DispatchSocket {
    
    class TCPSocket : public Socket {
        
    public:
#pragma mark - Consturctor & destructor
        TCPSocket();
        ~TCPSocket();
        TCPSocket(const TCPSocket&) = delete;
        TCPSocket& operator = (const TCPSocket&) = delete;
        
#pragma mark - Server
        bool sockListen();//开始监听,socket状态转成被动型
        bool sockListen(const uint16_t& port);//在指定端口号监听
        void shutdown();//关闭服务器,结束listen
        int currentConnectedSocketsCount() const;//获取当前连接的客户端数量
        
#pragma mark - Client
        bool sockConnect(const std::string& host,const uint16_t& port);//通过主机地址和端口号链接
        bool sockDisconnect();//断开连接
        
#pragma mark - I/O
        ssize_t sockRead(int fd, void* buffer,size_t length);//读
        ssize_t sockWrite(int fd, void* buffer,size_t length); //写
        
#pragma mark - Setter & Getter
        void setSockFd (const int& fd);//设置socket文件描述符
        void setAddressFamily(const int& af);//获取socket地址协议族类型
        
        int getSockFd() const;//获取socket文件描述符
        int getSockAddressFamily() const;//设置socket地址协议族类型
        dispatch_queue_t getSockQueue() const;//获取socket连接的队列
        
    private:
        int _sockFd;//socket文件描述符
        int _addressFamily;//地址协议族
        std::vector<TCPSocket *> _connectedSockets;//如果是服务端，存放已经连接的客户端Socket
        
        dispatch_source_t _accpetSource;//接收事件的dipatch source
        dispatch_source_t _readSource;//读事件的dipatch source
        dispatch_source_t _writeSource;//写事件的dipatch source
        dispatch_queue_t _sockQueue;//socket连接的队列，一个串行队列
        
        bool sockClose(const int& fd);//通过socket文件描述符关闭socket
        void acceptHandler(const int& fd,const std::string& url);//接收事件处理
        void setupReadAndWriteSource(const int& fd,const std::string& url);//设置读写事件
        
        std::shared_ptr<SocketDataPacket> _currentRead;
    };
}

#endif /* TCPSocket_hpp */
