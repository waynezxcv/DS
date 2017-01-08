
#import "AppDelegate.h"
#import "DispatchTCPSocket.h"



@interface AppDelegate ()<DispatchTCPSocketDelegate>

@property (weak) IBOutlet NSWindow *window;
@property (nonatomic,strong) DispatchTCPSocket* server;
@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    self.server = [[DispatchTCPSocket alloc] initWithDelegate:self];
    [self.server listen];
}


@end
