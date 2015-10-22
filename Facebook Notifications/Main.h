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
#include <chrono>
#include "AppDelegateBridge.h"
#include "Parser.h"
#include "ImageCache.h"
#include "AccessTokenStorage.h"
#include "FacebookException.h"

#define INITIAL_UPDATE_TIME 1
#define MAX_UPDATE_TIME     15

class Main
{
private:
    static AppDelegateBridge *bridge;
    static Request *request;
    static Parser *parser;
    static ImageCache *cache;
    static AccessTokenStorage *tokenStorage;
    static Notifications notifications;
    static std::string accessToken;
    static User user;
    static std::size_t readMessagesTime;
    static std::chrono::minutes updateTime;
    
    Main();
    static std::chrono::minutes nextUpdateTime(const Notifications);
    
public:
    static int main(AppDelegateBridge *);
    static void markNotificationRead(void *);
    static void markNotificationsRead(void *);
    static void reauthenticate(void *);
};

#endif
