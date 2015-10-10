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
    
    Request request;
    Parser parser;
    
    try {
        std::stringstream buffer;
        request.request("/me", &buffer);
        parser.parse(&buffer);
    } catch (std::runtime_error e) {
        cout << e.what() << std::endl;
    }
    
    return 0;
}