
#import "AppDelegate.h"
#import "DispatchSocket.h"

@interface AppDelegate ()

@property (weak) IBOutlet NSWindow *window;
@property (nonatomic,strong) LWTcpServer* server;
@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    self.server = [[LWTcpServer alloc] init];
    [self.server listen];
}



@end
