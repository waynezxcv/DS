//
//  LWTcpServer.h
//  DispatchSocketDemo
//
//  Created by 刘微 on 2017/1/7.
//  Copyright © 2017年 WayneInc. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface LWTcpServer : NSObject

- (id)init;
- (void)listen;
- (void)listenOnPort:(NSInteger)port;

@end
