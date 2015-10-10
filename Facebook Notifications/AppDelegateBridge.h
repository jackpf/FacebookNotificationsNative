//
//  AppDelegateBridge.h
//  Facebook Notifications
//
//  Created by Jack Farrelly on 09/10/2015.
//  Copyright (c) 2015 Jack Farrelly. All rights reserved.
//

#ifndef Facebook_Notifications_AppDelegateBridge_h
#define Facebook_Notifications_AppDelegateBridge_h

#import <Foundation/Foundation.h>
#include <iostream>

@interface AppDelegateBridgeNative : NSObject

-(void) notify:(NSString *)title :(NSString *)body;

@end

class AppDelegateBridge
{
private:
    AppDelegateBridgeNative *bridge;
    
public:
    AppDelegateBridge(AppDelegateBridgeNative *);
    
    void notify(std::string, std::string);
};

#endif
