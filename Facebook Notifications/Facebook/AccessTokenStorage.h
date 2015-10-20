//
//  AccessTokenStorage.h
//  Facebook Notifications
//
//  Created by Jack Farrelly on 14/10/2015.
//  Copyright (c) 2015 Jack Farrelly. All rights reserved.
//

#ifndef __Facebook_Notifications__AccessTokenStorage__
#define __Facebook_Notifications__AccessTokenStorage__

#include <iostream>
#include <fstream>
#include <vector>
#include <boost/filesystem.hpp>
#include <map>
#include <regex>
#include "Request.h"
#include "Data.h"

class AccessTokenStorage
{
private:
    std::string storageFile;
    
public:
    AccessTokenStorage();
    void store(std::string);
    std::string read();
    std::string getAccessTokenFromCode(std::string) throw(std::runtime_error);
    std::string getCodeFromUrl(std::string) throw(std::runtime_error);
};

#endif /* defined(__Facebook_Notifications__AccessTokenStorage__) */
