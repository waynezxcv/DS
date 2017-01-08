
#import "AppDelegate.h"
#import "DispatchTCPSocket.h"



@interface AppDelegate ()

@property (weak) IBOutlet NSWindow *window;
@property (nonatomic,strong) DispatchTCPSocket* server;
@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    self.server = [[DispatchTCPSocket alloc] init];
    [self.server listen];
}


@end
