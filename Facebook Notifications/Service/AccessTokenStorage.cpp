//
//  AccessTokenStorage.cpp
//  Facebook Notifications
//
//  Created by Jack Farrelly on 14/10/2015.
//  Copyright (c) 2015 Jack Farrelly. All rights reserved.
//

#include "AccessTokenStorage.h"

AccessTokenStorage::AccessTokenStorage()
{
    std::string storageFileDir = std::string(getenv("HOME")) + "/.FacebookNotifications";
    
    if (!boost::filesystem::exists(storageFileDir)) {
        boost::filesystem::create_directories(storageFileDir);
    }
    
    storageFile = storageFileDir + "/access_token";
}

void AccessTokenStorage::store(std::string accessToken)
{
    std::ofstream out(storageFile);
    out << accessToken;
    out.close();
}

std::string AccessTokenStorage::read()
{
    std::string accessToken;
    
    std::ifstream in(storageFile, std::ios::in);
    in >> accessToken;
    in.close();
    
    return accessToken;
}