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


#include "TCPServer.hpp"


using namespace DispatchSocket;

#pragma mark - TCPServer

TCPServer::TCPServer( PacketEncoder& encoder, PacketDecoder& decoder) : _encoder(encoder),_decoder(decoder) {
    _streamObserver = new ServerStreamObserver();
    _socketObserver = new ServerSocketObserver();
    _socket = new TCPSocket(_socketObserver,_streamObserver);
}

TCPServer::~TCPServer() {
    delete _socket;
    delete _socketObserver;
    delete _streamObserver;
}

void TCPServer::serverListen() {
    _socket -> sockListen();
}

void TCPServer::serverListen(const uint16_t& port) {
    _socket->sockListen(port);
}

void TCPServer::serverShutdown() {
    _socket->shutdown();
}

unsigned TCPServer::currentConnectsCount() {
    return _socket->currentConnectedSocketsCount();
}

#pragma mark - Strem Observer

void ServerStreamObserver::hasBytesAvailable(TCPSocket* socket,const dispatch_queue_t& queue) {
    uint8_t buffer[1024];
    ssize_t len = socket->sockRead(buffer, 1024);


}


void ServerStreamObserver::hasSpaceAvailable(TCPSocket* socket,const dispatch_queue_t& queue) {
    uint8_t buffer[1024];
    ssize_t len = socket->sockWrite(buffer, 1024);
    
}

void ServerStreamObserver::readEOF(TCPSocket* socket,const dispatch_queue_t& queue) {
    
}


void ServerStreamObserver::writeEOF(TCPSocket* socket,const dispatch_queue_t& queue) {
    
}


void ServerStreamObserver::errorOccurred(TCPSocket* socket) {
    printf("error occurred!\n");
}

#pragma mark - Socket Observer
void ServerSocketObserver::didAcceptNewClient(TCPSocket* server,TCPSocket* client) {
    std::string ip;
    uint16_t port;
    client->sockGetPeerName(client->getSockFd(), ip, port);
#ifdef DEBUG
    std::cout<<"*** *** *** *** ***"<<std::endl;
    std::cout<<"accept!  connect Fd:"<<client->getSockFd()<<std::endl;
    std::cout<<"client address:"<<ip<<":"<<port<<std::endl;
    std::cout<<"*** *** *** *** ***"<<std::endl;
#endif
}


void ServerSocketObserver::aClientDidDisconnected(const std::string& clientURL) {



}


