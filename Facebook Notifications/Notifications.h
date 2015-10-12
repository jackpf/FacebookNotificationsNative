//
//  Notifications.h
//  Facebook Notifications
//
//  Created by Jack Farrelly on 12/10/2015.
//  Copyright (c) 2015 Jack Farrelly. All rights reserved.
//

#ifndef __Facebook_Notifications__Notifications__
#define __Facebook_Notifications__Notifications__

#include <iostream>
#include <vector>
#include "Notification.h"

class Notifications : public std::vector<Notification>
{
private:
    Notifications *oldNotifications;
    
public:
    void reset();
    bool hasNew();
    bool containsNotification(Notification);
    Notifications getNew();
};

#endif /* defined(__Facebook_Notifications__Notifications__) */
