//
//  Main.cpp
//  Facebook Notifications
//
//  Created by Jack Farrelly on 09/10/2015.
//  Copyright (c) 2015 Jack Farrelly. All rights reserved.
//

#include "Main.h"

int Main::main(AppDelegateBridge *bridge)
{
    //std::string accessToken = bridge->getInput("Access token:");
    std::string accessToken = "CAAI9MvHB7MwBALX1nAzjrGu4JYORd5JmKZCueAPNKnIVZC2eVt8gf7AXhmqacL8PjRCggkSOSyistuDdgQwix4z0uZA50PWMESMMd3LvwwVd33LsJenUD6fQP02ywwzZAGqqhGviKrCLXd5BH2BWrF9kS8oBBDZCRW3KsWj1OzpzS6jslFtaR";
    
    if (accessToken.compare("") == 0) {
        throw std::runtime_error("Empty access token");
    }
    
    Request request(accessToken);
    Parser parser;
    
    try {
        std::stringstream buffer;
        Notifications notifications;
        
        while (true) {
            request.request("/me/notifications", &buffer);
            parser.parseNotifications(&buffer, &notifications);
            Notifications newNotifications = notifications.getNew();
            
            cout << "Found " << notifications.size() << " notifications, " << newNotifications.size() << " new" << std::endl;
            
            for(Notifications::iterator it = newNotifications.begin(); it != newNotifications.end(); ++it) {
                Notification notification = static_cast<Notification>(*it);
                
                bridge->notify(notification.get("id"), notification.get("title"));
            }
            
            sleep(10);
        }
    } catch (std::runtime_error e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }
    
    return 0;
}