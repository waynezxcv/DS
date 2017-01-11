
#import "AppDelegate.h"
#import "TCPSocket.hpp"
#import "CustomLengthEncoder.hpp"
#import "CustomLengthDecoder.hpp"




using namespace DispatchSocket;

@interface AppDelegate (){
    
    TCPSocket* _server;
    CustomLengthEncoder* _encoder;
    CustomLengthDecoder* _decoder;
    
}


@property (weak) IBOutlet NSWindow *window;

@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    _encoder = new CustomLengthEncoder();
    _decoder = new CustomLengthDecoder();
    _server = new TCPSocket(_encoder,_decoder);
    _server->sockListen(58953);
}

- (void)dealloc {
    delete _server;
    delete _encoder;
    delete _decoder;
}



@end
