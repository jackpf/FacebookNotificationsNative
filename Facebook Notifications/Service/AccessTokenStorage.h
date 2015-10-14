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
#include <boost/filesystem.hpp>

class AccessTokenStorage
{
private:
    std::string storageFile;
    
public:
    AccessTokenStorage();
    void store(std::string);
    std::string read();
};

#endif /* defined(__Facebook_Notifications__AccessTokenStorage__) */
