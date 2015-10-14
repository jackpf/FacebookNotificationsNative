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

int Main::main(AppDelegateBridge *bridge)
{
    Main::bridge = bridge;
    bridge->addEvent("markNotificationsRead", &markNotificationsRead);
    
    std::string accessToken = bridge->getInput("Access token:");
    //std::string accessToken = "CAAI9MvHB7MwBALX1nAzjrGu4JYORd5JmKZCueAPNKnIVZC2eVt8gf7AXhmqacL8PjRCggkSOSyistuDdgQwix4z0uZA50PWMESMMd3LvwwVd33LsJenUD6fQP02ywwzZAGqqhGviKrCLXd5BH2BWrF9kS8oBBDZCRW3KsWj1OzpzS6jslFtaR";
    
    if (accessToken.compare("") == 0) {
        throw std::runtime_error("Empty access token");
    }
    
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
            
            bridge->setNotificationCount(static_cast<int>(notifications.size()));
            
            for(Notifications::iterator it = newNotifications.begin(); it != newNotifications.end(); ++it) {
                Notification notification = static_cast<Notification>(*it);
                
                bridge->notify(notification.get("id"), notification.get("title"), cache.fetch(notification.get("from")));
            }
        } catch (std::runtime_error e) {
            std::cout << "Caught exception: " << e.what() << std::endl;
            bridge->alert(e.what());
        }
            
        sleep(10);
    }
    
    delete request;
    delete parser;
    
    return 0;
}