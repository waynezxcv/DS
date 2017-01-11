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

TCPServer::TCPServer() {
    _socket = new TCPSocket();
    setupCallBack();
}

//TCPServer::TCPServer( PacketEncoder& encoder, PacketDecoder& decoder) : _encoder(encoder),_decoder(decoder) {
//    _socket = new TCPSocket();
//}

void TCPServer::setupCallBack() {
    _socket->startListenCallBack = std::bind(&TCPServer::startListenCallBack,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
    _socket->didAcceptNewClientCallBack = std::bind(&TCPServer::didAcceptNewClientCallBack,this,std::placeholders::_1,std::placeholders::_2);
    _socket->hasBytesAvailableCallBack = std::bind(&TCPServer::hasBytesAvailableCallBack, this,std::placeholders::_1,std::placeholders::_2);
    _socket->hasSpaceAvailableCallBack = std::bind(&TCPServer::hasSpaceAvailableCallBack, this,std::placeholders::_1,std::placeholders::_2);
    _socket->readEOFCallBack = std::bind(&TCPServer::readEOFCallBack,this,std::placeholders::_1,std::placeholders::_2);
    _socket->writeEOFCallBack = std::bind(&TCPServer::writeEOFCallBack, this,std::placeholders::_1,std::placeholders::_2);
    _socket->errorOccuerred = std::bind(&TCPServer::errorOccuerred, this,std::placeholders::_1);
}

void TCPServer::startListenCallBack(TCPSocket* tcpSock,const std::string& host,const uint16_t& port) {
    std::cout<<"start listen! host:" <<host<<"  port:"<<port<<std::endl;
}

void TCPServer::didAcceptNewClientCallBack(TCPSocket* server,TCPSocket* client) {
    std::cout<<"accept a new client!"<<std::endl;
}

void TCPServer::hasBytesAvailableCallBack(TCPSocket* tcpSock,const dispatch_queue_t& queue) {
    std::cout<<"has bytes available!"<<std::endl;
    
}

void TCPServer::hasSpaceAvailableCallBack(TCPSocket* tcpSock,const dispatch_queue_t& queue) {
    std::cout<<"has space available!"<<std::endl;
    
}
void TCPServer::readEOFCallBack(TCPSocket* tcpSock,const dispatch_queue_t& queue) {
    std::cout<<"read EOF!"<<std::endl;
    
}

void TCPServer::writeEOFCallBack(TCPSocket* tcpSock,const dispatch_queue_t& queue) {
    std::cout<<"write EOF!"<<std::endl;
    
}

void TCPServer::errorOccuerred(TCPSocket* tcpSock) {
    std::cout<<"error occueered!"<<std::endl;
    
}

TCPServer::~TCPServer() {
    delete _socket;
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
