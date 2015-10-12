//
//  Notification.cpp
//  Facebook Notifications
//
//  Created by Jack Farrelly on 12/10/2015.
//  Copyright (c) 2015 Jack Farrelly. All rights reserved.
//

#include "Notification.h"

std::string Notification::get(std::string key)
{
    for(iterator it = begin(); it != end(); ++it) {
        std::pair<std::string, std::string> pv = static_cast<std::pair<std::string, std::string>>(*it);
        
        if (key.compare(pv.first.data()) == 0) {
            return pv.second.data();
        }
    }
    
    return "";
}