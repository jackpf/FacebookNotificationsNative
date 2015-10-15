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
Notifications Main::notifications;

int Main::main(AppDelegateBridge *bridge)
{
    Main::bridge = bridge;
    bridge->addEvent("markNotificationsRead", &markNotificationsRead);
    
    AccessTokenStorage tokenStorage;
    std::string accessToken = tokenStorage.read();
    
    request = new Request(accessToken);
    parser = new Parser;
    ImageCache cache(request);
    
    std::stringstream buffer;
    
    while (true) {
        try {
            request->request("/me/notifications", &buffer);
            parser->parseNotifications(&buffer, &notifications);
            Notifications newNotifications = notifications.getNew();
            
            cout << "Found " << notifications.size() << " notifications, " << newNotifications.size() << " new" << std::endl;
            
            bridge->setNotificationCount(notifications.size());
            
            for(Notifications::iterator it = newNotifications.begin(); it != newNotifications.end(); ++it) {
                Notification notification = static_cast<Notification>(*it);
                
                bridge->notify(notification.get("id"), notification.get("from"), notification.get("title"), cache.fetch(notification.get("from_id")));
            }
        } catch (std::runtime_error e) {
            std::cout << "Caught exception: " << e.what() << std::endl;
            bridge->alert(e.what());
        }
            
        sleep(60);
    }
    
    //delete request;
    //delete parser;
    
    return 0;
}

void Main::markNotificationsRead(void *data)
{
    int i = 0;
    
    for(Notifications::iterator it = notifications.begin(); it != notifications.end(); ++it) {
        Notification notification = static_cast<Notification>(*it);
        
        request->request("/" + notification.get("id"), std::vector<std::string>{"unread=false"}, true, nullptr);
        i++;
    }
    
    notifications.reset();
    bridge->setNotificationCount(0);
    
    std::cout << "Marked " << i << " notifications as read" << std::endl;
}