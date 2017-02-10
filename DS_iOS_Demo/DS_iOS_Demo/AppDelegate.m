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

#import "AppDelegate.h"
#import "TCPSocketMananger.h"




@interface AppDelegate ()<TCPSocketManangerDelegate>

@property (nonatomic,strong) TCPSocketMananger* tcpManager;

@end

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    self.tcpManager = [[TCPSocketMananger alloc] initWithDelegate:self];
    [self.tcpManager connectToHost:@"192.168.1.101" port:63873];
    return YES;
}



//连接到指定ip和端口号成功回调
- (void)didConnectedToHost:(NSString *)host port:(NSInteger)port {
    NSLog(@"tcp client did connected to host:%@ port:%ld success",host,port);
    
    
    NSString* str = @"hello,waynezcv!";
    NSData* data = [str dataUsingEncoding:NSUTF8StringEncoding];
    [self.tcpManager writeData:data type:1024 timeout:-1];
}

//连接到指定ip和端口号失败回调
- (void)connectedFailedToHost:(NSString *)host port:(NSInteger)port {
    NSLog(@"tcp client did connected to host:%@ port:%ld failed",host,port);
}

//接收到数据回调
- (void)didReceivedData:(NSData *)data type:(NSInteger)type {
    NSLog(@"did received data :%@ type :%ld",data,type);
}


@end
