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
    bridge->notify("Title", "Body");
    bridge->notify("Another", "Hi");
    
    Request request("CAAI9MvHB7MwBALX1nAzjrGu4JYORd5JmKZCueAPNKnIVZC2eVt8gf7AXhmqacL8PjRCggkSOSyistuDdgQwix4z0uZA50PWMESMMd3LvwwVd33LsJenUD6fQP02ywwzZAGqqhGviKrCLXd5BH2BWrF9kS8oBBDZCRW3KsWj1OzpzS6jslFtaR");
    Parser parser;
    
    try {
        std::stringstream buffer;
        request.request("/me/notifications", &buffer);
        parser.parse(&buffer);
    } catch (std::runtime_error e) {
        cout << "Caught exception: " << e.what() << std::endl;
    }
    
    return 0;
}