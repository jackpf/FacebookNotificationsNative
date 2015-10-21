//
//  Main.cpp
//  Facebook Notifications
//
//  Created by Jack Farrelly on 09/10/2015.
//  Copyright (c) 2015 Jack Farrelly. All rights reserved.
//

#include "Main.h"

AppDelegateBridge *Main::bridge;
Request *Main::request;
Parser *Main::parser;
ImageCache *Main::cache;
Notifications Main::notifications;
std::string Main::accessToken;

int Main::main(AppDelegateBridge *bridge)
{
    Main::bridge = bridge;
    bridge->addEvent("markNotificationRead", &markNotificationRead);
    bridge->addEvent("markNotificationsRead", &markNotificationsRead);
    
    AccessTokenStorage tokenStorage;
    accessToken = tokenStorage.read();
    
    request = Request::getInstance();
    cache = new ImageCache;
    parser = new Parser;
    
    std::cout << "Pruned " << cache->prune() << " cached images" << std::endl;
    
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
                
                std::string code = tokenStorage.getCodeFromUrl(bridge->retrieveAuthenticationCode());
                accessToken = tokenStorage.getAccessTokenFromCode(code);
                tokenStorage.store(accessToken);
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
            
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
    
    //delete request;
    //delete parser;
    //delete cache;
    
    return 0;
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