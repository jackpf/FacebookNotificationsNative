//
//  FacebookException.h
//  
//
//  Created by Jack Farrelly on 21/10/2015.
//
//

#ifndef ____FacebookException__
#define ____FacebookException__

#include <iostream>
#include <exception>

class FacebookDefaultException : public std::runtime_error
{
public:
    FacebookDefaultException(std::string what) : std::runtime_error(what){}
};

class FacebookLoginException : public FacebookDefaultException
{
public:
    FacebookLoginException(std::string what) : FacebookDefaultException(what){}
};

class FacebookException
{
public:
    static FacebookDefaultException *createFromErrorMessage(std::string message)
    {
        // Should be defined somewhere else really
        const std::string
            ERROR_ACCESS_TOKEN          = "An active access token must be used to query information about the current user.",
            ERROR_EXTENDED_PERMISSION   = "Requires extended permission";
        
        if (message.find(ERROR_ACCESS_TOKEN) != std::string::npos) {
            return new FacebookLoginException(message);
        } else if (message.find(ERROR_EXTENDED_PERMISSION) != std::string::npos) {
            return new FacebookLoginException(message);
        } else {
            return new FacebookDefaultException(message);
        }
    }
};

#endif /* defined(____FacebookException__) */
