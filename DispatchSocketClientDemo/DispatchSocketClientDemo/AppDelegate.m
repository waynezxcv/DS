

#import "AppDelegate.h"
#import "DispatchSocket.h"

@interface AppDelegate ()

@property (nonatomic,strong) LWTcpClient* tcpClient;

@end


@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    self.tcpClient = [[LWTcpClient alloc] init];
    [self.tcpClient connectToHost:@"192.168.1.101" onPort:61883];
    
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(2.0F * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        [self.tcpClient disConnect];
    });
    return YES;
}


@end
