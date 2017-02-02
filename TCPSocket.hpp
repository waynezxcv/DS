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
#include "DataPacket.hpp"
#include <queue>
#include <map>


typedef struct {
    bool socketOpened : 1;
    bool acceptSourceOpend : 1;
    bool readSourceOpened  : 1;
    bool writeSourceOpened : 1;
    bool readSourceSuspend : 1;
    bool writeSourceSuspend : 1;
    bool hasReadEOF : 1;
    bool hasBytesAvailable : 1;
    bool hasWriteSpaceAvailable : 1;
    bool isWritting : 1;
    
} SocketFlags;


typedef enum {
    TCPSocketRoleServer,
    TCPSocketRoleClient
} TCPSocketRole;


namespace DispatchSocket {
    class TCPSocket : public Socket {
        
    public:
        
        explicit TCPSocket();
        virtual ~TCPSocket();
        TCPSocket(const TCPSocket&) = delete;
        TCPSocket& operator = (const TCPSocket&) = delete;
        
        bool sockListen();
        bool sockListen(const uint16_t& port);
        void sockConnect(const std::string& host,const uint16_t& port);
        bool sockDisconnect();
        bool sockDisconnect(const std::string& url);
        void shutdown();
        void writeData(const Data& data,int type);
        
        std::shared_ptr<TCPSocket> tcpsocketForURL(const std::string& URL);
        unsigned currentConnectedSocketsCount() const;
        TCPSocketRole role () const;
        
        std::function<void(const std::string&,const uint16_t&)> startListenCallBack;
        std::function<void(const std::string&)> acceptANewClientCallback;
        std::function<void(const std::string&,const uint16_t&)> didConnectedToHostSuccessCallBack;
        std::function<void(std::shared_ptr<Data>, const int& type)> didReceivedDataCallBack;
        
    private:
        SocketFlags _flags;
        TCPSocketRole _role;
        int _sockFd;
        int _addressFamily;
        std::string _URL;
        std::map<std::string,std::shared_ptr<TCPSocket>> _connectedSockets;
        std::queue<std::shared_ptr<WritePacket>> _writeQueue;
        std::shared_ptr<WritePacket> _currentWrite;
        std::shared_ptr<Data> _currentRead;
        unsigned long _sockBytesAvaliabled;
        TCPSocket* _parentSocket;
        dispatch_source_t _accpetSource;
        dispatch_source_t _readSource;
        dispatch_source_t _writeSource;
        dispatch_queue_t _sockQueue;
        dispatch_semaphore_t _semaphore;
        
        void suspendReadSource();
        void suspendWriteSource();
        void resumeReadSource();
        void resumeWriteSource();
        int getSockFd() const;
        bool sockClose(const int& fd);
        void acceptHandler(const int& fd,const std::string& url);
        void setupReadAndWriteSource(const int& fd,const std::string& url);
        void setSockFd (const int& fd);
        void setAddressFamily(const int& af);
        int getSockAddressFamily() const;
        dispatch_queue_t getSockQueue() const;
        void readDataHandler();
        void writeDataHanlder();
        void readEOFHandler();
    };
}



#endif /* TCPSocket_hpp */
