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

void Parser::parseNotifications(std::stringstream *json, Notifications *data) throw(std::runtime_error)
{
    boost::property_tree::ptree pt;
    parseJson(json, &pt);
    
    data->reset();
    
    BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("data")) {
        Notification notification;
        
        BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, v.second) {
            std::pair<std::string, std::string> pv;
            
            if (strcmp(v2.first.data(), "from") == 0 || strcmp(v2.first.data(), "to") == 0) {
                // Meh, a bit shitty...
                notification.push_back(std::make_pair(std::string(v2.first.data()), v2.second.get_child("name").data()));
                notification.push_back(std::make_pair(std::string(v2.first.data()) + "_id", v2.second.get_child("id").data()));
            } else {
                notification.push_back(std::make_pair(v2.first.data(), v2.second.data()));
            }
        }
        
        data->push_back(notification);
    }
}