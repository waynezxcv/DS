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



#import "TCPSocketMananger.h"
#import "TCPSocket.hpp"
#import "Data.hpp"



@implementation TCPSocketMananger {
    DispatchSocket::TCPSocket* _socket;
}

#pragma mark - Callback

- (void)startListenCallBack:(NSString *)ip port:(NSInteger)port {
    if (self.delegate && [self.delegate respondsToSelector:@selector(didStartListenWithIP:port:)]) {
        [self.delegate didStartListenWithIP:ip port:port];
    }
}


- (void)acceptANewClientCallback:(NSString *)URL {
    if (self.delegate && [self.delegate respondsToSelector:@selector(didAcceptANewClient:)]) {
        [self.delegate didAcceptANewClient:URL];
    }
}

- (void)didConnectedToHostSuccessCallBack:(NSString *)ip port:(NSInteger)port {
    if (self.delegate && [self.delegate respondsToSelector:@selector(didConnectedToHost:port:)]) {
        [self.delegate didConnectedToHost:ip port:port];
    }
}

- (void)didReceivedDataCallBack:(NSData *)data type:(int)type {
    if (self.delegate && [self.delegate respondsToSelector:@selector(didReceivedData:type:)]) {
        [self.delegate didReceivedData:data type:type];
    }
}

#pragma mark - LifeCycle

- (id)init {
    self = [super init];
    if (self) {
        _socket = new DispatchSocket::TCPSocket();
        [self setupCallback];
    }
    return self;
}

- (id)initWithDelegate:(id<TCPSocketManangerDelegate>)delegate {
    self = [super init];
    if (self) {
        _socket = new DispatchSocket::TCPSocket();
        _delegate = delegate;
        [self setupCallback];
    }
    return self;
}


- (void)setupCallback {
    
    _socket->startListenCallBack = [self](const std::string& ip,const uint16_t& port) -> void {
        [self startListenCallBack:[[NSString alloc] initWithUTF8String:ip.c_str()] port:port];
    };
    
    _socket->acceptANewClientCallback = [self] (const std::string& url) -> void {
        [self acceptANewClientCallback:[[NSString alloc] initWithUTF8String:url.c_str()]];
        
        std::shared_ptr<DispatchSocket::TCPSocket> newSokcet = _socket->tcpsocketForURL(url);
        newSokcet->didReceivedDataCallBack = [self] (std::shared_ptr<DispatchSocket::Data> payload, const int& type) -> void {
            uint8_t* buffer = (uint8_t *)malloc(payload -> length() * sizeof(uint8_t));
            for (int i = 0; i < payload -> length(); i ++) {
                memcpy(buffer + i,&payload -> bytes()[i], sizeof(uint8_t));
            }
            
            NSData* data = [[NSData alloc] initWithBytes:buffer length:payload -> length()];
            [self didReceivedDataCallBack:data type:type];
            free(buffer);
        };
    };
    
    _socket -> didConnectedToHostSuccessCallBack = [self] (const std::string& host,const uint16_t& port) -> void {
        [self didConnectedToHostSuccessCallBack:[[NSString alloc] initWithUTF8String:host.c_str()] port:port];
    };
}

- (void)dealloc {
    delete _socket;
}

#pragma mark - Methods

- (void)listen {
    _socket->sockListen();
}

- (void)listenOnPort:(NSInteger)port {
    _socket->sockListen(port);
}

- (void)connectToHost:(NSString *)host port:(NSInteger)port {
    _socket->sockConnect(std::string([host UTF8String]), port);
}

- (void)disConnect {
    _socket->sockDisconnect();
}

- (void)disConnectAClientWithURL:(NSString *)clientURL {
    _socket->sockDisconnect(std::string([clientURL UTF8String]));
}

- (void)shutdown {
    _socket->shutdown();
}

- (void)writeData:(NSData *)data type:(NSInteger)type toHost:(NSString *)host port:(NSInteger)port timeout:(double)timeout {
    if (_socket->role() == TCPSocketRoleServer) {
        NSString* url = [NSString stringWithFormat:@"%@:%ld",host,port];
        std::shared_ptr<DispatchSocket::TCPSocket> socket = _socket->tcpsocketForURL(std::string([url UTF8String]));
        if (socket == nullptr) {
            return;
        }
        DispatchSocket::Data d = DispatchSocket::Data((uint8_t *)data.bytes,data.length);
        socket->writeData(d, (int)type);
    }
}

- (void)writeData:(NSData *)data type:(NSInteger)type timeout:(double)timeout {
    DispatchSocket::Data d = DispatchSocket::Data((uint8_t *)data.bytes,data.length);
    _socket->writeData(d, (int)type);
}

- (NSInteger)currentConnectedSocketsCount {
    return _socket->currentConnectedSocketsCount();
}

@end
