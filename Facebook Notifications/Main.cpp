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
            request->request("/me/notifications", Request::Params{Request::Param("access_token", accessToken)}, &buffer, true);
            
            parser->parseNotifications(&buffer, &notifications);
            Notifications newNotifications = notifications.getNew();
            
            std::cout << "Found " << notifications.size() << " notifications, " << newNotifications.size() << " new" << std::endl;
            
            bridge->updateNotificationCount(notifications.size());
            
            for(Notifications::iterator it = newNotifications.begin(); it != newNotifications.end(); ++it) {
                auto notification = static_cast<Notification>(*it);
                
                bridge->notify(notification.get("id"), notification.get("from"), notification.get("title"), notification.get("link"), cache->fetch(notification.get("from_id")));
            }
        } catch (const FacebookDefaultException *e) {
            if (auto real = dynamic_cast<const FacebookLoginException *>(e)) {
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
            bridge->alert(e.what());
        }
            
        std::this_thread::sleep_for(std::chrono::seconds(60));
    }
    
    return 0;
}

void Main::reauthenticate(void *data)
{
    std::string code = tokenStorage->getCodeFromUrl(bridge->retrieveAuthenticationCode());
    accessToken = tokenStorage->getAccessTokenFromCode(code);
    tokenStorage->store(accessToken);
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
        
        request->request("/" + notification.get("id"), Request::Params{Request::Param("unread", "false"), Request::Param("access_token", accessToken)}, true, nullptr, false /*no auto lock*/);
        i++;
    }
    
    notifications.reset();
    bridge->updateNotificationCount(0);
    
    request->mutex.unlock();
    
    std::cout << "Marked " << i << " notifications as read" << std::endl;
}