//
//  AppDelegate.m
//  DispatchSocketDemo
//
//  Created by 刘微 on 2017/1/7.
//  Copyright © 2017年 WayneInc. All rights reserved.
//

#import "AppDelegate.h"
#import "LWTcpServer.h"

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
