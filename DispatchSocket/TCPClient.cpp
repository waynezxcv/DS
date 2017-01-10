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

TCPClient::TCPClient(PacketEncoder& encoder, PacketDecoder& decoder) : _encoder(encoder),_decoder(decoder) {
    _socketObserver = new ClientSocketObserver();
    _streamObserver = new ClientStreamObserver();
    _socket = new DispatchSocket::TCPSocket(_socketObserver,_streamObserver);
}


TCPClient::~TCPClient() {
    delete _socket;
    delete _socketObserver;
    delete _streamObserver;
}

void TCPClient::clientConnect(const std::string& host,const int& port) {
    _socket->sockConnect(host, port);
}


void TCPClient::clientDisconnect() {
    _socket->sockDisconnect();
}


#pragma mark - Strem Observer

void ClientStreamObserver::hasBytesAvailable(TCPSocket* socket,const dispatch_queue_t& queue) {
    
}

void ClientStreamObserver::hasSpaceAvailable(TCPSocket* socket,const dispatch_queue_t& queue) {
    
}


void ClientStreamObserver::readEOF(TCPSocket* socket,const dispatch_queue_t& queue) {
    
}


void ClientStreamObserver::writeEOF(TCPSocket* socket,const dispatch_queue_t& queue) {
    
}


void ClientStreamObserver::errorOccurred(TCPSocket* socket) {
    
}


#pragma mark - Socket Observer

void ClientSocketObserver::didConnected(const std::string& host,const uint16_t& port) {
#ifdef DEBUG
    std::cout<<"*** *** *** *** ***"<<std::endl;
    std::cout<<"host  "<<host<<":"<<port<<std::endl;
    std::cout<<"*** *** *** *** ***"<<std::endl;
#endif
}


void ClientSocketObserver::didDisconnected() {
    
}




