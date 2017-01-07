//
//  LWTcpServer.m
//  DispatchSocketDemo
//
//  Created by 刘微 on 2017/1/7.
//  Copyright © 2017年 WayneInc. All rights reserved.
//

#import "LWTcpServer.h"
#import "TCPServer.hpp"

using namespace DispatchSocket;

@interface LWTcpServer () {
    TCPServer* _server;
}

@end

@implementation LWTcpServer

- (id)init {
    self = [super init];
    if (self) {
        _server = new TCPServer();
    }
    return self;
}

- (void)listen {
    _server->sockListen();
}

- (void)listenOnPort:(NSInteger)port {
    _server->sockListen(port);
}

- (void)dealloc {
    delete _server;
}

@end
