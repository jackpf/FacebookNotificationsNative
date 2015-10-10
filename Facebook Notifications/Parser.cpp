//
//  Parser.cpp
//  Facebook Notifications
//
//  Created by Jack Farrelly on 09/10/2015.
//  Copyright (c) 2015 Jack Farrelly. All rights reserved.
//

#include "Parser.h"

void Parser::parseJson(std::stringstream *json, boost::property_tree::ptree *pt) throw(std::runtime_error)
{
    boost::property_tree::read_json(*json, *pt);
    
    if (pt->count("error") > 0) {
        throw std::runtime_error("Facebook error: " + pt->get<std::string>("error.message")); // Should be FacebookException
    }
}

void Parser::parseNotifications(std::stringstream *json, Parser::Notifications *data) throw(std::runtime_error)
{
    boost::property_tree::ptree pt;
    parseJson(json, &pt);
    
    BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("data")) {
        std::vector<std::pair<std::string, std::string>> notification;
        
        BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, v.second) {
            std::pair<std::string, std::string> pv(v2.first.data(), v2.second.data());
            notification.push_back(pv);
        }
        
        data->push_back(notification);
    }
}