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


#import <Foundation/Foundation.h>

@protocol DSObjcInterfaceDelegate <NSObject>

@optional

//开始监听回调
- (void)didStartListenWithIP:(NSString *)ip port:(NSInteger)port;

//接受了一个新的客户端连接回调
- (void)didAcceptANewClient:(NSString *)clientURL;

//连接到指定ip和端口号成功回调
- (void)didConnectedToHost:(NSString *)host port:(NSInteger)port;

//接收到数据回调
- (void)didReceivedData:(NSData *)data type:(NSInteger)type;

@end



@interface DispatchSocketObjc : NSObject

@property (nonatomic,weak) id<DSObjcInterfaceDelegate> delegate;

//构造函数，指定代理
- (id)initWithDelegate:(id<DSObjcInterfaceDelegate>)delegate;

//开始监听
- (void)listen;

//在指定端口号监听
- (void)listenOnPort:(NSInteger)port;

//连接到指定的ip和端口号
- (void)connectToHost:(NSString *)host port:(NSInteger)port;

//断开连接，客户端调用
- (void)disConnect;

//断开连接，服务端调用
- (void)disConnectAClientWithURL:(NSString *)clientURL;

//关闭，服务端调用
- (void)shutdown;

//指定对端ip和端口号发送数据。服务端调用
- (void)writeData:(NSData *)data type:(NSInteger)type toHost:(NSString *)host port:(NSInteger)port timeout:(double)timeout;

//不指定对端ip和端口号发送数据。如果是客户端，则发送给建立连接的服务端，如果是服务端，则发送给所有建立连接的客户端
- (void)writeData:(NSData *)data type:(NSInteger)type timeout:(double)timeout;

//当前连接的客户端数量，服务端调用
- (NSInteger)currentConnectedSocketsCount;


@end
