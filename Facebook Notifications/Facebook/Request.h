//
//  Request.h
//  Facebook Notifications
//
//  Created by Jack Farrelly on 10/10/2015.
//  Copyright (c) 2015 Jack Farrelly. All rights reserved.
//

#ifndef __Facebook_Notifications__Request__
#define __Facebook_Notifications__Request__

#include <stdio.h>
#include <iostream>
#include <exception>
#include <sstream>
#include <vector>
#include "curlcpp/curl_easy.h"

class Request
{
private:
    const std::string BASE_URL = "https://graph.facebook.com";
    
    std::string accessToken;
    
public:
    Request(std::string);
    void request(const std::string, std::vector<std::string>, bool, std::ostream *) throw(std::runtime_error);
    void request(const std::string, std::ostream *) throw(std::runtime_error);
};

#endif /* defined(__Facebook_Notifications__Request__) */
