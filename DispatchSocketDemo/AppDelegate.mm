
#import "AppDelegate.h"
#import "DispatchSocket.h"



using namespace DispatchSocket;

@interface AppDelegate (){
    TCPServer* _server;
}


@property (weak) IBOutlet NSWindow *window;

@end


@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    _server = new TCPServer();
    _server->serverListen(58953);
    
}


- (void)dealloc {

}



@end
