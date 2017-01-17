
#import "ViewController.h"
#import "DSObjcInterface.h"




@interface ViewController ()<DSObjcInterfaceDelegate>

@property (nonatomic,strong) DispatchSocketObjc* socket;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    self.socket = [[DispatchSocketObjc alloc] initWithDelegate:self];
    [self.socket connectToHost:@"192.168.1.223" port:59269];

    UIButton* button = [UIButton buttonWithType:UIButtonTypeCustom];
    [button setTitle:@"发送" forState:UIControlStateNormal];
    [button setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    button.frame = CGRectMake(0, 0, 100, 40);
    button.center = self.view.center;
    [self.view addSubview:button];
    [button addTarget:self action:@selector(didClickedButton)
     forControlEvents:UIControlEventTouchUpInside];

}


- (void)didClickedButton {

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
    NSLog(@"did received data !");
}


@end
