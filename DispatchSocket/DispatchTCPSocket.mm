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


#import "DispatchTCPSocket.h"
#import "TCPSocket.hpp"



void evnentHandler(void* socket, TCPSocketEvent event);

@interface DispatchTCPSocket () {
    DispatchSocket::TCPSocket* _tcpSocket;
}

@end

@implementation DispatchTCPSocket

- (id)init {
    self = [super init];
    if (self) {
        _tcpSocket = new DispatchSocket::TCPSocket();
    }
    return self;
}


- (id)initWithDelegate:(id<DispatchTCPSocketDelegate>)delegate {
    self = [super init];
    if (self) {
        _tcpSocket = new DispatchSocket::TCPSocket();
        _tcpSocket->sockhandleEvent = evnentHandler;

        self.delegate = delegate;
    }
    return self;
}


- (void)dealloc {
    delete _tcpSocket;
}

- (void)connectToHost:(NSString *)host onPort:(NSInteger)port {
    std::string ip ([host UTF8String]);
    _tcpSocket->sockConnect(ip, port);
}

- (void)disconnect {
    _tcpSocket->sockDisconnect();
}

- (void)listen {
    _tcpSocket->sockListen();
}
- (void)listenOnPort:(NSInteger)port {
    _tcpSocket->sockListen(port);
}

- (void)shutdown {
    _tcpSocket->shutdown();
}

- (NSInteger)getCurrentConnectedSocketsCount {
    return _tcpSocket->currentConnectedSocketsCount();
}

@end


void evnentHandler(void* socket, TCPSocketEvent event) {
    printf("event:%u",event);
    
}

