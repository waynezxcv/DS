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
    
    
    class TCPServer {
    public:
        TCPServer();
//        TCPServer(PacketEncoder& encoder, PacketDecoder& decoder);
        ~TCPServer();
        TCPServer(const TCPServer&) = delete;
        TCPServer& operator = (const TCPServer&) = delete;
        
        void serverListen();
        void serverListen(const uint16_t& port);
        void serverShutdown();
        unsigned currentConnectsCount();
        
        
        void setupCallBack();
        void startListenCallBack(TCPSocket*,const std::string&,const uint16_t&);
        void didAcceptNewClientCallBack(TCPSocket*,TCPSocket*);
        void hasBytesAvailableCallBack(TCPSocket*,const dispatch_queue_t&);
        void hasSpaceAvailableCallBack(TCPSocket*,const dispatch_queue_t&);
        void readEOFCallBack(TCPSocket*,const dispatch_queue_t&);
        void writeEOFCallBack(TCPSocket*,const dispatch_queue_t&);
        void errorOccuerred(TCPSocket*);
        
    private:
        TCPSocket* _socket;
//        PacketEncoder& _encoder;
//        PacketDecoder& _decoder;
    };
    
}


#endif /* TCPServer_hpp */
