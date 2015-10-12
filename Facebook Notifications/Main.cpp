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
    std::string accessToken = bridge->getInput("Access token:");
    
    Request request(accessToken);
    Parser parser;
    
    try {
        std::stringstream buffer;
        Parser::Notifications notifications;
        
        request.request("/me/notifications", &buffer);
        
        parser.parseNotifications(&buffer, &notifications);
        
        for(Parser::Notifications::iterator it = notifications.begin(); it != notifications.end(); ++it) {
            Parser::Notification notification = static_cast<Parser::Notification>(*it);
            
            std::string title, body;
            
            for(Parser::Notification::iterator it2 = notification.begin(); it2 != notification.end(); ++it2) {
                std::pair<std::string, std::string> pv = static_cast<std::pair<std::string, std::string>>(*it2);
                
                if (strcmp(pv.first.data(), "id") == 0) {
                    title = pv.second.data();
                } else if(strcmp(pv.first.data(), "title") == 0) {
                    body = pv.second.data();
                }
            }
            
            bridge->notify(title, body);
        }
    } catch (std::runtime_error e) {
        cout << "Caught exception: " << e.what() << std::endl;
    }
    
    return 0;
}