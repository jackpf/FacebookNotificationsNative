//
//  ImageCache.cpp
//  Facebook Notifications
//
//  Created by Jack Farrelly on 12/10/2015.
//  Copyright (c) 2015 Jack Farrelly. All rights reserved.
//

#include "ImageCache.h"

ImageCache::ImageCache(Request *request)
{
    this->request = request;
    cacheDir = std::string(getenv("HOME")) + "/.FacebookNotifications/cache";
    
    if (!boost::filesystem::exists(cacheDir)) {
        boost::filesystem::create_directories(cacheDir);
    }
}

std::string ImageCache::getFilename(const std::string userId)
{
    return cacheDir + "/" + userId + ".jpg";
}

bool ImageCache::isCached(const std::string userId)
{
    long t = std::time(nullptr) - boost::filesystem::last_write_time(getFilename(userId));
    return boost::filesystem::exists(getFilename(userId)) && t < CACHE_TIME;
}

void ImageCache::cache(const std::string userId)
{
    std::string filename = getFilename(userId);
    std::fstream picture(filename, std::ios::out | std::ios::binary);
    
    request->request("/" + userId + "/picture", std::vector<std::string>{"width=128", "height=128"}, false, &picture);
    
    picture.close();
}

std::string ImageCache::fetch(const std::string userId)
{
    if (!isCached(userId)) {
        std::cout << "Caching " + getFilename(userId) << std::endl;
        cache(userId);
    } else {
        std::cout << getFilename(userId) << " loaded from cache" << std::endl;
    }
    
    return getFilename(userId);
}