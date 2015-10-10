//
//  Notification.h
//  Facebook Notifications
//
//  Created by Jack Farrelly on 09/10/2015.
//  Copyright (c) 2015 Jack Farrelly. All rights reserved.
//

#ifndef Facebook_Notifications_Notification_h
#define Facebook_Notifications_Notification_h

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#include <iostream>

class Notification
{
public:
    void notify(std::string, std::string);
};

#endif
