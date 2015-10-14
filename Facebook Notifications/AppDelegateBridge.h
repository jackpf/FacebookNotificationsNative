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
#include <unordered_map>
#include <thread>

class AppDelegateBridge;

@interface AppDelegateBridgeNative : NSObject <NSApplicationDelegate, NSUserNotificationCenterDelegate>

@property (nonatomic) AppDelegateBridge *bridge;
@property (strong, nonatomic) NSStatusItem *statusBar;
@property (nonatomic) NSZone *menuZone;
@property (strong, nonatomic) NSMenu *menu;
@property (strong, nonatomic) NSMenuItem *markNotificationsReadMenuItem, *settingsMenuItem, *exitMenuItem;

- (AppDelegateBridgeNative *) init;
- (void) setNotificationCount :(unsigned long)count;
- (void) notify :(NSString *)identifier :(NSString *)title :(NSString *)body :(NSString *)image;
- (void) getInput :(NSString *)prompt :(NSString **) r;
- (void) alert :(NSString *)prompt;
- (void) markNotificationsRead;
- (void) exit;

@end

class AppDelegateBridge
{
private:
    AppDelegateBridgeNative *bridge;
    
public:
    typedef void (EventCallback)(void *);
    std::unordered_map<std::string, EventCallback *> callbacks;
    
    void setBridge(AppDelegateBridgeNative *);
    void setNotificationCount(size_t);
    void notify(std::string, std::string, std::string, std::string = "");
    std::string getInput(std::string);
    void alert(std::string);
    
    void addEvent(std::string, EventCallback);
    void removeEvent(std::string);
    void event(std::string, void * = NULL);
};

#endif
