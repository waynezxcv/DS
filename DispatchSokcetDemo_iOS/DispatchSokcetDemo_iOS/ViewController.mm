
#import "ViewController.h"
#import "TCPSocketMananger.h"




@interface ViewController ()<TCPSocketManangerDelegate>

@property (nonatomic,strong) TCPSocketMananger* socket;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.view.backgroundColor = [UIColor grayColor];
    
    
    UIButton* button = [UIButton buttonWithType:UIButtonTypeCustom];
    [button setTitle:@"发送" forState:UIControlStateNormal];
    [button setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    button.frame = CGRectMake(0, 0, 100, 40);
    button.center = self.view.center;
    [button addTarget:self action:@selector(didClickedButton)
     forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:button];
    
    
    self.socket = [[TCPSocketMananger alloc] initWithDelegate:self];
    [self.socket connectToHost:@"192.168.1.101" port:59269];
}


- (void)didClickedButton {
    
    //    UIImage* image = [UIImage imageNamed:@"test"];
    //    NSData* imgData = UIImagePNGRepresentation(image);
    //    NSLog(@"size :%ld",imgData.length);
    
    NSString* str = @"hello,dispatch socket!";
    NSData* strData = [str dataUsingEncoding:NSUTF8StringEncoding];
    
    
    [self.socket writeData:strData type:123 timeout:0];
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
