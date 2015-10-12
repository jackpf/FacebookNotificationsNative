//
//  AppDelegateBridge.h
//  Facebook Notifications
//
//  Created by Jack Farrelly on 09/10/2015.
//  Copyright (c) 2015 Jack Farrelly. All rights reserved.
//

#ifndef Facebook_Notifications_AppDelegateBridge_h
#define Facebook_Notifications_AppDelegateBridge_h

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#include <iostream>

@interface AppDelegateBridgeNative : NSObject <NSApplicationDelegate, NSUserNotificationCenterDelegate>

@property (strong, nonatomic) IBOutlet NSMenu *statusMenu;
@property (strong, nonatomic) NSStatusItem *statusBar;

-(void) initialise;
-(void) notify: (NSString *)title :(NSString *)body;
- (NSString *) getInput: (NSString *)prompt defaultValue: (NSString *)defaultValue;

@end

class AppDelegateBridge
{
private:
    AppDelegateBridgeNative *bridge;
    
public:
    AppDelegateBridge(AppDelegateBridgeNative *);
    
    void initialise();
    void notify(std::string, std::string);
    std::string getInput(std::string);
};

#endif
