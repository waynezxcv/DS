

#import "AppDelegate.h"
#import "DispatchTCPSocket.h"

@interface AppDelegate ()

@property (nonatomic,strong) DispatchTCPSocket* tcpClient;

@end


@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    self.tcpClient = [[DispatchTCPSocket alloc] init];
    [self.tcpClient connectToHost:@"192.168.1.101" onPort:58921];
    return YES;
}

@end
