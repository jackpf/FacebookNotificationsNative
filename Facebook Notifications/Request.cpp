//
//  Request.cpp
//  Facebook Notifications
//
//  Created by Jack Farrelly on 10/10/2015.
//  Copyright (c) 2015 Jack Farrelly. All rights reserved.
//

#include "Request.h"

static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    std::ostream *response = static_cast<std::ostream *>(userdata);
    response->write(ptr, size * nmemb);
    
    return size * nmemb;
}

void Request::request(const std::string path, std::ostream *response) throw(std::runtime_error)
{
    CURL *ch;
    CURLcode res;
    
    if (!(ch = curl_easy_init())) {
        throw std::runtime_error("Request: Unable to initialise curl");
    }
    
    std::string url = BASE_URL + path;
    
    curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(ch, CURLOPT_WRITEDATA, response);
    curl_easy_setopt(ch, CURLOPT_URL, url.c_str());
    
    if ((res = curl_easy_perform(ch)) != CURLE_OK) {
        std::string errStr = "Request: " + std::string(strerror(res));
        throw std::runtime_error(errStr);
    }
    
    curl_easy_cleanup(ch);
}