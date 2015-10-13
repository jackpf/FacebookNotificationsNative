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

class AppDelegateBridge;

@interface AppDelegateBridgeNative : NSObject <NSApplicationDelegate, NSUserNotificationCenterDelegate>

@property (nonatomic) AppDelegateBridge *bridge;
@property (strong, nonatomic) NSStatusItem *statusBar;

- (AppDelegateBridgeNative *) init;
- (void) setNotificationCount :(int)count;
- (void) notify :(NSString *)title :(NSString *)body :(NSString *)image;
- (NSString *) getInput :(NSString *)prompt :(NSString *)defaultValue;
- (void) markNotificationsRead;
- (void) exit;

@end

class AppDelegateBridge
{
private:
    AppDelegateBridgeNative *bridge;
    
public:
    void setBridge(AppDelegateBridgeNative *);
    void setNotificationCount(int);
    void notify(std::string, std::string, std::string = "");
    std::string getInput(std::string);
    void markNotificationsRead();
};

#endif
