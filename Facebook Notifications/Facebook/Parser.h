//
//  Parser.h
//  Facebook Notifications
//
//  Created by Jack Farrelly on 09/10/2015.
//  Copyright (c) 2015 Jack Farrelly. All rights reserved.
//

#ifndef __Facebook_Notifications__Parser__
#define __Facebook_Notifications__Parser__

#include <iostream>
#include <exception>
#include <sstream>
#include <vector>
#include <algorithm>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include "Request.h"
#include "Notifications.h"

class Parser
{
private:
    void parseJson(std::stringstream *, boost::property_tree::ptree *) throw(std::runtime_error);
    
public:
    void parseNotifications(std::stringstream *, Notifications *) throw(std::runtime_error);
};

#endif /* defined(__Facebook_Notifications__Parser__) */
