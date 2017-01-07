//
//  LWTcpClient.m
//  DispatchSocketDemo
//
//  Created by 刘微 on 2017/1/7.
//  Copyright © 2017年 WayneInc. All rights reserved.
//

#import "LWTcpClient.h"
#import "TCPClient.hpp"


using namespace DispatchSocket;


@interface LWTcpClient () {
    TCPClient* _client;
}

@end



@implementation LWTcpClient


- (id)init {
    self = [super init];
    if (self) {
        _client = new TCPClient();
    }
    return self;
}

- (void)connectToHost:(NSString *)host onPort:(NSInteger)port {
    std::string str (host.cString);
    _client->sockConnect(str, port);
}

- (void)dealloc {
    delete _client;
}

@end
