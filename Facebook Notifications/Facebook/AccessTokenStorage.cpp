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

std::string AccessTokenStorage::getCodeFromUrl(std::string url) throw(std::runtime_error)
{
    std::regex rgx("code=(.+)(&|#|$)");
    std::smatch match;
    
    if (!std::regex_search(url, match, rgx) || match.length() < 1) {
        throw std::runtime_error("Unable to extract code");
    }
    
    std::cout << "Code: " << match[1] << std::endl;
    
    return match[1];
}

std::string AccessTokenStorage::getAccessTokenFromCode(std::string code) throw(std::runtime_error)
{
    std::stringstream buffer;
    
    Request::getInstance()->request("/oauth/access_token", Request::Params{Request::Param("client_id", CLIENT_ID), Request::Param("client_secret", CLIENT_SECRET), Request::Param("redirect_uri", REDIRECT_URI), Request::Param("code", code)}, &buffer);
    
    std::regex rgx("access_token=(.+)(&|$)");
    std::smatch match;
    
    std::string str = buffer.str();
    if (!std::regex_search(str, match, rgx) || match.length() < 1) {
        throw std::runtime_error("Unable to extract access token");
    }
    
    std::cout << "Access token: " << match[1] << std::endl;
    
    return match[1];
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