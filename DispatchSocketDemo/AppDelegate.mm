
#import "AppDelegate.h"
#import "TCPSocketMananger.h"
#import "Data.hpp"

@interface AppDelegate ()<TCPSocketManangerDelegate>


@property (weak) IBOutlet NSWindow *window;
@property (nonatomic,strong) TCPSocketMananger* socket;

@end

@implementation AppDelegate


- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    
    self.socket = [[TCPSocketMananger alloc] initWithDelegate:self];
    [self.socket listenOnPort:59269];
    
}

- (void)didStartListenWithIP:(NSString *)ip port:(NSInteger)port {
    NSLog(@"did start listen with ip : %@ port: %ld",ip,port);
}


- (void)didAcceptANewClient:(NSString *)clientURL {
    NSLog(@"server did accept a new client : %@",clientURL);
}


- (void)didConnectedToHost:(NSString *)host port:(NSInteger)port {
    NSLog(@"client did connected to host : %@ port : %ld",host,port);
}


- (void)didReceivedData:(NSData *)data type:(NSInteger)type {
    NSLog(@"did received data ! type :%ld .. data:%@",(long)type,data);
    NSString* receivedStr = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    NSLog(@"%@",receivedStr);
}


@end
