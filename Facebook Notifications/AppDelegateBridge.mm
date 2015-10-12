//
//  AppDelegateBridge.m
//  Facebook Notifications
//
//  Created by Jack Farrelly on 09/10/2015.
//  Copyright (c) 2015 Jack Farrelly. All rights reserved.
//

#include "AppDelegateBridge.h"

@implementation AppDelegateBridgeNative

-(void) initialise
{
    [[NSUserNotificationCenter defaultUserNotificationCenter] setDelegate:self];
    
    self.statusBar = [[NSStatusBar systemStatusBar] statusItemWithLength:NSSquareStatusItemLength];
    //self.statusBar.title = @"F";
    self.statusBar.image = [NSImage imageNamed:@"notification_light"];
    self.statusBar.menu = self.statusMenu;
    self.statusBar.highlightMode = YES;
}

-(void)notify:(NSString *) title :(NSString *)body;
{
    NSUserNotification *notification = [[NSUserNotification alloc] init];
    notification.title = title;
    notification.informativeText = body;
    notification.soundName = NSUserNotificationDefaultSoundName;
    [[NSUserNotificationCenter defaultUserNotificationCenter]
     deliverNotification:notification];
}

- (NSString *)getInput: (NSString *)prompt
{
    NSAlert *alert = [NSAlert alertWithMessageText: prompt
                                     defaultButton:@"OK"
                                   alternateButton:@"Cancel"
                                       otherButton:nil
                         informativeTextWithFormat:@""];
    
    NSTextField *input = [[NSTextField alloc] initWithFrame:NSMakeRect(0, 0, 200, 24)];
    //[input setStringValue:defaultValue];
    //[input autorelease];
    [alert setAccessoryView:input];
    NSInteger button = [alert runModal];
    if (button == NSAlertDefaultReturn) {
        [input validateEditing];
        return [input stringValue];
    } else if (button == NSAlertAlternateReturn) {
        return nil;
    } else {
        NSAssert1(NO, @"Invalid input dialog button %d", button);
        return nil;
    }
}

@end

AppDelegateBridge::AppDelegateBridge(AppDelegateBridgeNative *bridge)
{
    this->bridge = bridge;
}

void AppDelegateBridge::initialise()
{
    [bridge initialise];
}

void AppDelegateBridge::notify(std::string title, std::string body)
{
    [bridge notify:[NSString stringWithUTF8String:title.c_str()] :[NSString stringWithUTF8String:body.c_str()]];
}

std::string AppDelegateBridge::getInput(std::string prompt)
{
    return [[bridge getInput:[NSString stringWithUTF8String:prompt.c_str()]] UTF8String];
}