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

#include "TCPSocket.hpp"
#include "PacketEncoder.hpp"
#include "PacketDecoder.hpp"


namespace DispatchSocket {
    
    class ServerStreamObserver;
    class ServerSocketObserver;

    class TCPServer {
    public:
        TCPServer(PacketEncoder& encoder, PacketDecoder& decoder);
        ~TCPServer();
        TCPServer(const TCPServer&) = delete;
        TCPServer& operator = (const TCPServer&) = delete;
        
        void serverListen();
        void serverListen(const uint16_t& port);
        void serverShutdown();
        unsigned currentConnectsCount();
        
        
        
        
        //call back
        void (*didAcceptNewClientCallback)(std::string clientURL);
        
        void (*didReceiveDataCallBack)(void* data);
        
        
    private:
        TCPSocket* _socket;
        ServerSocketObserver* _socketObserver;
        ServerStreamObserver* _streamObserver;
        PacketEncoder& _encoder;
        PacketDecoder& _decoder;
    };
    
    
    
    class ServerStreamObserver : public StreamEventObserver {
    public:
        ServerStreamObserver() {};
        ~ServerStreamObserver(){};
        
        void hasBytesAvailable(TCPSocket* socket,const dispatch_queue_t& queue) override;
        void hasSpaceAvailable(TCPSocket* socket,const dispatch_queue_t& queue) override;
        void readEOF(TCPSocket* socket,const dispatch_queue_t& queue) override;
        void writeEOF(TCPSocket* socket,const dispatch_queue_t& queue) override;
        void errorOccurred(TCPSocket* socket) override;
    };
        
    class ServerSocketObserver : public TCPServerEventObserver {
    public:
        ServerSocketObserver(){};
        ~ServerSocketObserver(){};
        void didAcceptNewClient(TCPSocket* server,TCPSocket* client) override;
        void aClientDidDisconnected(const std::string& clientURL) override;
    };
}


#endif /* TCPServer_hpp */
