//
//  Main.cpp
//  Facebook Notifications
//
//  Created by Jack Farrelly on 09/10/2015.
//  Copyright (c) 2015 Jack Farrelly. All rights reserved.
//

#include "Main.h"

AppDelegateBridge   *Main::bridge;
Request             *Main::request      = Request::getInstance();
Parser              *Main::parser       = new Parser;
ImageCache          *Main::cache        = new ImageCache;
AccessTokenStorage  *Main::tokenStorage = AccessTokenStorage::getInstance();
Notifications       Main::notifications;
std::string         Main::accessToken;
User                Main::user;

int Main::main(AppDelegateBridge *bridge)
{
    Main::bridge = bridge;
    bridge->addEvent("markNotificationRead", &markNotificationRead);
    bridge->addEvent("markNotificationsRead", &markNotificationsRead);
    bridge->addEvent("reauthenticate", &reauthenticate);
    
    std::cout << "Pruned " << cache->prune() << " cached images" << std::endl;
    
    accessToken = tokenStorage->read();
    std::stringstream buffer;
    
    while (true) {
        try {
            if (user.id.empty()) {
                std::cout << "Loading user data" << std::endl;
                request->request("/me", Request::Params{Request::Param("access_token", accessToken)}, &buffer, true);
                parser->parseUser(&buffer, &user);
            }
            
            request->request("/me/notifications", Request::Params{Request::Param("access_token", accessToken)}, &buffer, true);
            parser->parseNotifications(&buffer, &notifications);
            
            //request->request("/me/inbox", Request::Params{Request::Param("access_token", accessToken)}, &buffer, true);
            //parser->parseUnreadMessages(&buffer, &notifications, 1444867200, user);
            
            Notifications newNotifications = notifications.getNew();
            
            std::cout << "Found " << notifications.size() << " notifications, " << newNotifications.size() << " new" << std::endl;
            
            bridge->updateNotificationCount(notifications.size());
            
            for(Notifications::iterator it = newNotifications.begin(); it != newNotifications.end(); ++it) {
                auto notification = static_cast<Notification>(*it);
                
                bridge->notify(notification.id, notification.title, notification.body, notification.link, cache->fetch(notification.from));
            }
        } catch (const FacebookDefaultException *e) {
            if (auto real = dynamic_cast<const FacebookAuthenticationException *>(e)) {
                std::cout << "Login error: " << real->what() << std::endl;
                reauthenticate(nullptr);
            } else {
                std::string errMsg = std::string("Facebook error: ") + e->what();
                std::cout << errMsg << std::endl;
                bridge->alert(errMsg);
            }
            delete e;
        } catch (const std::runtime_error &e) {
            std::string errMsg = std::string("Runtime error: ") + e.what();
            std::cout << errMsg << std::endl;
            bridge->alert(errMsg);
        }
            
        std::this_thread::sleep_for(std::chrono::seconds(60));
    }
    
    return 0;
}

void Main::reauthenticate(void *data)
{
    try {
        std::string code = tokenStorage->getCodeFromUrl(bridge->retrieveAuthenticationCode());
        accessToken = tokenStorage->getAccessTokenFromCode(code);
        tokenStorage->store(accessToken);
    } catch (FacebookLoginException *e) {
        std::string errMsg = std::string("Login error: ") + e->what();
        std::cout << errMsg << std::endl;
        bridge->alert(errMsg);
        delete e;
    }
}

void Main::markNotificationRead(void *data)
{
    request->request("/" + *static_cast<std::string *>(data), Request::Params{Request::Param("unread", "false"), Request::Param("access_token", accessToken)}, true, nullptr, true);
}

void Main::markNotificationsRead(void *data)
{
    request->mutex.lock();
    
    int i = 0;
    
    for(Notifications::iterator it = notifications.begin(); it != notifications.end(); ++it) {
        auto notification = static_cast<Notification>(*it);
        
        if (notification.type == NotificationType::NOTIFICATION) {
            request->request("/" + notification.id, Request::Params{Request::Param("unread", "false"), Request::Param("access_token", accessToken)}, true, nullptr, false /*no auto lock*/);
            i++;
        }
    }
    
    /* SHOULD UPDATE LAST UPDATE TIME FOR MESSAGES HERE */
    
    notifications.reset();
    bridge->updateNotificationCount(0);
    
    request->mutex.unlock();
    
    std::cout << "Marked " << i << " notifications as read" << std::endl;
}