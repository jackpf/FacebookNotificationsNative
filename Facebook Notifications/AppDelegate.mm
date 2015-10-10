//
//  AppDelegate.m
//  Facebook Notifications
//
//  Created by Jack Farrelly on 09/10/2015.
//  Copyright (c) 2015 Jack Farrelly. All rights reserved.
//

#import "AppDelegate.h"

@implementation AppDelegate

- (BOOL)userNotificationCenter:(NSUserNotificationCenter *)center
     shouldPresentNotification:(NSUserNotification *)notification
{
    return YES;
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application
    [[NSUserNotificationCenter defaultUserNotificationCenter] setDelegate:self];
    
    self.statusBar = [[NSStatusBar systemStatusBar] statusItemWithLength:NSSquareStatusItemLength];
    //self.statusBar.title = @"F";
    self.statusBar.image = [NSImage imageNamed:@"notification_light"];
    self.statusBar.menu = self.statusMenu;
    self.statusBar.highlightMode = YES;
    
    AppDelegateBridge *bridge = new AppDelegateBridge([AppDelegateBridgeNative alloc]);
    Main::main(bridge);
    delete bridge;
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}

@end
