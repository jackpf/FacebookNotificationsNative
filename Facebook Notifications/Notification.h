//
//  Notification.h
//  Facebook Notifications
//
//  Created by Jack Farrelly on 12/10/2015.
//  Copyright (c) 2015 Jack Farrelly. All rights reserved.
//

#ifndef __Facebook_Notifications__Notification__
#define __Facebook_Notifications__Notification__

#include <iostream>
#include <vector>

class Notification : public std::vector<std::pair<std::string, std::string>>
{
public:
    std::string get(const std::string);
};

#endif /* defined(__Facebook_Notifications__Notification__) */
