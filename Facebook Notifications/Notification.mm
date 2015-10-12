//
//  Notification.m
//  Facebook Notifications
//
//  Created by Jack Farrelly on 09/10/2015.
//  Copyright (c) 2015 Jack Farrelly. All rights reserved.
//

#import "Notification_.h"

void Notification::notify(std::string title, std::string body)
{
    NSUserNotification *notification = [[NSUserNotification alloc] init];
    notification.title = [NSString stringWithUTF8String:title.c_str()];
    notification.informativeText = [NSString stringWithUTF8String:body.c_str()];
    notification.soundName = NSUserNotificationDefaultSoundName;
    [[NSUserNotificationCenter defaultUserNotificationCenter]
        deliverNotification:notification];
}