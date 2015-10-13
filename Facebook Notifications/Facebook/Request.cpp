//
//  Request.cpp
//  Facebook Notifications
//
//  Created by Jack Farrelly on 10/10/2015.
//  Copyright (c) 2015 Jack Farrelly. All rights reserved.
//

#include "Request.h"

static size_t write_callback(char *ptr, size_t size, size_t nmemb, std::ostream *userdata)
{
    size_t len = size * nmemb;
    
    if (userdata != nullptr) {
        userdata->write(ptr, len);
        userdata->flush();
    }
    
    return len;
}

Request::Request(std::string accessToken)
{
    this->accessToken = accessToken;
}

void Request::request(const std::string path, std::vector<std::string> params, bool post, std::ostream *response) throw(std::runtime_error)
{
    CURL *ch;
    CURLcode res;
    
    if (!(ch = curl_easy_init())) {
        throw std::runtime_error("Request: Unable to initialise curl");
    }
    
    std::string url = BASE_URL + path + "?access_token=" + accessToken;
    
    for(std::vector<std::string>::iterator it = params.begin(); it != params.end(); ++it) {
        url += "&" + std::string(*it);
    }
    
    curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(ch, CURLOPT_WRITEDATA, response);
    curl_easy_setopt(ch, CURLOPT_URL, url.c_str());
    curl_easy_setopt(ch, CURLOPT_FOLLOWLOCATION, 1);
    
    if (post) {
        curl_easy_setopt(ch, CURLOPT_POST, 1);
        curl_easy_setopt(ch, CURLOPT_POSTFIELDS, "");
        curl_easy_setopt(ch, CURLOPT_POSTFIELDSIZE, 0);
    }
    
    if ((res = curl_easy_perform(ch)) != CURLE_OK) {
        throw std::runtime_error("Request: " + std::string(strerror(res)));
    }
    
    curl_easy_cleanup(ch);
}

void Request::request(const std::string path, std::ostream *response) throw(std::runtime_error)
{
    request(path, std::vector<std::string>(), false, response);
}