//
//  Main.cpp
//  Facebook Notifications
//
//  Created by Jack Farrelly on 09/10/2015.
//  Copyright (c) 2015 Jack Farrelly. All rights reserved.
//

#include "Main.h"
#include "Notification.h"

int Main::main()
{
    std::cout << "Notification!";
    
    Notification::notify("Title", "Body");
    
    return 0;
}