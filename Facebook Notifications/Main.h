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
#include "AccessTokenStorage.h"

class Main
{
private:
    static AppDelegateBridge *bridge;
    static Request *request;
    static Parser *parser;
    static ImageCache *cache;
    static Notifications notifications;
    
public:
    static int main(AppDelegateBridge *);
    static void markNotificationsRead(void *);
};

#endif
