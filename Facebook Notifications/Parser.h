//
//  Parser.h
//  Facebook Notifications
//
//  Created by Jack Farrelly on 09/10/2015.
//  Copyright (c) 2015 Jack Farrelly. All rights reserved.
//

#ifndef __Facebook_Notifications__Parser__
#define __Facebook_Notifications__Parser__

#include <boost/property_tree/json_parser.hpp>
#include <iostream>
#include <exception>
#include <sstream>
#include "Request.h"

class Parser
{
public:
    void parse(std::stringstream *) throw(std::runtime_error);
};

#endif /* defined(__Facebook_Notifications__Parser__) */
