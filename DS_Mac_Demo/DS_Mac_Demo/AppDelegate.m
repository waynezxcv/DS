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




@interface AppDelegate () <TCPSocketManangerDelegate>

@property (weak) IBOutlet NSWindow *window;
@property (nonatomic,strong) TCPSocketMananger* tcpManager;


@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    self.tcpManager = [[TCPSocketMananger alloc] initWithDelegate:self];
    [self.tcpManager listenOnPort:63873];
}

#pragma mark  - TCPSocketManangerDelegate
//开始监听回调
- (void)didStartListenWithIP:(NSString *)ip port:(NSInteger)port {
    NSLog(@"tcp server did start listen on : %@:%ld",ip,port);
}

//接受了一个新的客户端连接回调
- (void)didAcceptANewClient:(NSString *)clientURL {
    NSLog(@"tcp server did accept a new client :%@",clientURL);
}

//接收到数据回调
- (void)didReceivedData:(NSData *)data type:(NSInteger)type {
    NSLog(@"did received data :\n%@ type :%ld",[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding],type);
}

@end
