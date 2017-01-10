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


#ifndef TCPClient_hpp
#define TCPClient_hpp

#include "TCPSocket.hpp"
#include "PacketEncoder.hpp"
#include "PacketDecoder.hpp"


namespace DispatchSocket {
    
    class ClientSocketObserver;
    class ClientStreamObserver;
    
    class TCPClient {
    public:
        TCPClient(PacketEncoder& encoder, PacketDecoder& decoder);
        ~TCPClient();
        TCPClient(const TCPClient&) = delete;
        TCPClient& operator = (const TCPClient&) = delete;
        
        void clientConnect(const std::string& host,const int& port);
        void clientDisconnect();
        
    private:
        DispatchSocket::TCPSocket* _socket;
        ClientSocketObserver* _socketObserver;
        StreamEventObserver* _streamObserver;
        PacketEncoder& _encoder;
        PacketDecoder& _decoder;
    };
    
    
    
    class ClientSocketObserver : public TCPClientObserver {
    public:
        ClientSocketObserver(){};
        ~ClientSocketObserver(){};
        
        void didConnected(const std::string& host,const uint16_t& port) override;
        void didDisconnected() override;
    };
    
    
    class ClientStreamObserver : public StreamEventObserver {
    public:
        
        ClientStreamObserver() {};
        ~ClientStreamObserver() {};
        
        void hasBytesAvailable(TCPSocket* socket,const dispatch_queue_t& queue) override;
        void hasSpaceAvailable(TCPSocket* socket,const dispatch_queue_t& queue) override;
        void readEOF(TCPSocket* socket,const dispatch_queue_t& queue) override;
        void writeEOF(TCPSocket* socket,const dispatch_queue_t& queue) override;
        void errorOccurred(TCPSocket* socket) override;
    };
    
    
}



#endif /* TCPClient_hpp */
