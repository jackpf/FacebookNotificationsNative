//
//  Parser.cpp
//  Facebook Notifications
//
//  Created by Jack Farrelly on 09/10/2015.
//  Copyright (c) 2015 Jack Farrelly. All rights reserved.
//

#include "Parser.h"

void Parser::parse(std::stringstream *json) throw(std::runtime_error)
{
    std::cout << json->str() << std::endl;
    
    boost::property_tree::ptree pt;
    boost::property_tree::read_json(*json, pt);
    std::cout << pt.get<std::string>("error.message") << std::endl;
}