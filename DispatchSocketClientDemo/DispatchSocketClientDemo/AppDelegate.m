

#import "AppDelegate.h"
#import "DispatchSocket.h"

@interface AppDelegate ()

@property (nonatomic,strong) LWTcpClient* tcpClient;

@end


@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    self.tcpClient = [[LWTcpClient alloc] init];
    [self.tcpClient connectToHost:@"192.168.1.101" onPort:60432];
    return YES;
}


@end
