//
//  Parser.cpp
//  Facebook Notifications
//
//  Created by Jack Farrelly on 09/10/2015.
//  Copyright (c) 2015 Jack Farrelly. All rights reserved.
//

#include "Parser.h"

void Parser::parseJson(std::stringstream *json, boost::property_tree::ptree *pt) throw(FacebookDefaultException *, std::runtime_error)
{
    boost::property_tree::read_json(*json, *pt);
    
    if (pt->count("error") > 0) {
        throw FacebookException::createFromErrorMessage(pt->get<std::string>("error.message"));
    }
}

std::time_t Parser::convertDate(std::string date)
{
    std::tm t;
    std::istringstream updatedTime(date);
    updatedTime >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S+0000");
    
    if (updatedTime.fail()) {
        std::cout << "Time parse failed: " << updatedTime.str() << std::endl;
        return 0;
    } else {
        return std::mktime(&t);
    }
}

void Parser::parseNotifications(std::stringstream *json, Notifications *data) throw(FacebookDefaultException *, std::runtime_error)
{
    boost::property_tree::ptree pt;
    parseJson(json, &pt);
    
    data->reset();
    
    BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("data")) {
        Notification notification;
        
        notification.id = v.second.get_child("id").data();
        notification.title = v.second.get_child("from.name").data();
        notification.body = v.second.get_child("title").data();
        notification.from = v.second.get_child("from.id").data();
        notification.link = v.second.get_child("link").data();
        notification.updatedAt = convertDate(v.second.get_child("updated_time").data());
        notification.type = NotificationType::NOTIFICATION;
        
        data->push_back(notification);
    }
}

void Parser::parseUnreadMessages(std::stringstream *json, Notifications *data, std::time_t tSince, User forUser) throw(FacebookDefaultException *, std::runtime_error)
{
    boost::property_tree::ptree pt;
    parseJson(json, &pt);
    
    BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("data")) {
        Notification notification;
        
        std::time_t t = convertDate(v.second.get<std::string>("updated_time").data());
        int unreadCount = v.second.get<int>("unread");
        
        if (unreadCount == 0) {
            continue;
        }
        
        if (t > tSince) {
            boost::property_tree::ptree users = v.second.get_child("to.data");
            
            BOOST_FOREACH(boost::property_tree::ptree::value_type &u, users) {
                if (forUser.id.compare(u.second.get_child("id").data()) != 0) {
                    notification.from = u.second.get_child("id").data();
                    notification.title = u.second.get_child("name").data();
                }
            }
            
            notification.id = v.second.get_child("id").data();
            notification.body = "New message!";
            notification.link = "";
            notification.updatedAt = convertDate(v.second.get_child("updated_time").data());
            notification.type = NotificationType::MESSAGE;
                
            data->push_back(notification);
        } else {
            break;
        }
    }
}

void Parser::parseUser(std::stringstream *json, User *user) throw(FacebookDefaultException *, std::runtime_error)
{
    boost::property_tree::ptree pt;
    parseJson(json, &pt);
    
    user->id = pt.get_child("id").data();
    user->name = pt.get_child("name").data();
}