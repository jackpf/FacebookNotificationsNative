//
//  Main.h
//  Facebook Notifications
//
//  Created by Jack Farrelly on 09/10/2015.
//  Copyright (c) 2015 Jack Farrelly. All rights reserved.
//

#ifndef Facebook_Notifications_Main_h
#define Facebook_Notifications_Main_h

#include <thread>
#include "AppDelegateBridge.h"
#include "Parser.h"
#include "ImageCache.h"

class Main
{
public:
    static int main(AppDelegateBridge *);
};

#endif
