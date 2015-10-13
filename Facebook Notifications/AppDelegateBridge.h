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

@property (strong, nonatomic) NSStatusItem *statusBar;

-(AppDelegateBridgeNative *) init;
-(void) setNotificationCount: (int)count;
-(void) notify: (NSString *)title :(NSString *)body :(NSString *)image;
-(NSString *) getInput: (NSString *)prompt defaultValue: (NSString *)defaultValue;
-(void) exit;

@end

class AppDelegateBridge
{
private:
    AppDelegateBridgeNative *bridge;
    
public:
    AppDelegateBridge(AppDelegateBridgeNative *);
    
    void setNotificationCount(int);
    void notify(std::string, std::string, std::string = "");
    std::string getInput(std::string);
};

#endif
