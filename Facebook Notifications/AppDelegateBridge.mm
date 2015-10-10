//
//  AppDelegateBridge.m
//  Facebook Notifications
//
//  Created by Jack Farrelly on 09/10/2015.
//  Copyright (c) 2015 Jack Farrelly. All rights reserved.
//

#include "AppDelegateBridge.h"

@implementation AppDelegateBridgeNative

-(void)notify:(NSString *) title :(NSString *)body;
{
    NSUserNotification *notification = [[NSUserNotification alloc] init];
    notification.title = title;
    notification.informativeText = body;
    notification.soundName = NSUserNotificationDefaultSoundName;
    [[NSUserNotificationCenter defaultUserNotificationCenter]
     deliverNotification:notification];
}

@end

AppDelegateBridge::AppDelegateBridge(AppDelegateBridgeNative *bridge)
{
    this->bridge = bridge;
}

void AppDelegateBridge::notify(std::string title, std::string body)
{
    [bridge notify:[NSString stringWithUTF8String:title.c_str()] :[NSString stringWithUTF8String:body.c_str()]];
}