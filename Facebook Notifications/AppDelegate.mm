//
//  AppDelegate.m
//  Facebook Notifications
//
//  Created by Jack Farrelly on 09/10/2015.
//  Copyright (c) 2015 Jack Farrelly. All rights reserved.
//

#import "AppDelegate.h"

@implementation AppDelegate

- (void)webView:(WebView *)sender didFinishLoadForFrame:(WebFrame *)frame
{
    NSString *url = [[[[frame dataSource] request] URL] absoluteString];
    printf("Load: %s\n", [url UTF8String]);
}

- (void) retrieveAuthenticationCode
{
    [self.webView setFrameLoadDelegate:self];
    NSURL *url = [NSURL URLWithString:@"https://www.facebook.com/dialog/oauth?client_id=630238967098572&redirect_uri=https://www.facebook.com/connect/login_success.html&scope=manage_notifications"];
    NSURLRequest *urlRequest = [NSURLRequest requestWithURL:url];
    [[[self webView] mainFrame] loadRequest:urlRequest];
    [self.window setContentView:self.webView];
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application
    [self performSelectorInBackground:@selector(run) withObject:nil];
    [self retrieveAuthenticationCode];
}

- (void)run {
    AppDelegateBridgeNative *nativeBridge = [[AppDelegateBridgeNative alloc] init];
    AppDelegateBridge *bridge = new AppDelegateBridge;
    
    [nativeBridge setBridge:bridge];
    bridge->setBridge(nativeBridge);
    
    Main::main(bridge);
    
    //delete bridge;
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}

@end
