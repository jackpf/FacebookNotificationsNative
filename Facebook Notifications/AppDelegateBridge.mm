//
//  AppDelegateBridge.m
//  Facebook Notifications
//
//  Created by Jack Farrelly on 09/10/2015.
//  Copyright (c) 2015 Jack Farrelly. All rights reserved.
//

#include "AppDelegateBridge.h"

@interface NSUserNotification (CFIPrivate)
    - (void)set_identityImage:(NSImage *)image;
@end

@implementation AppDelegateBridgeNative

- (BOOL) userNotificationCenter :(NSUserNotificationCenter *)center
     shouldPresentNotification:(NSUserNotification *)notification
{
    return YES;
}

- (AppDelegateBridgeNative *) init
{
    if (self = [super init]) {
        [[NSUserNotificationCenter defaultUserNotificationCenter] setDelegate:self];
    
        self.statusBar = [[NSStatusBar systemStatusBar] statusItemWithLength:NSSquareStatusItemLength];
        self.statusBar.image = [NSImage imageNamed:@"notification_light"];
        self.statusBar.alternateImage =[NSImage imageNamed:@"notification_alt"];
        self.statusBar.highlightMode = YES;
        
        NSZone *menuZone = [NSMenu menuZone];
        NSMenu *menu = [[NSMenu allocWithZone:menuZone] init];
        NSMenuItem *menuItem;
    
        menuItem = [menu addItemWithTitle:@"Mark all as read"
                                    action:@selector(markNotificationsRead)
                                    keyEquivalent:@""];
        [menuItem setTarget:self];
    
        [menu addItem:[NSMenuItem separatorItem]];
    
        menuItem = [menu addItemWithTitle:@"Exit"
                               action:@selector(exit)
                        keyEquivalent:@""];
        [menuItem setTarget:self];
    
        self.statusBar.menu = menu;
    }
    
    return self;
}

- (void) setNotificationCount :(int) count
{
    if (count > 0) {
        self.statusBar.image = [NSImage imageNamed:@"notification_dark"];
    } else {
        self.statusBar.image = [NSImage imageNamed:@"notification_light"];
    }
    
    self.statusBar.toolTip = [NSString stringWithFormat:@"%d notifications", count];
}

- (void) notify :(NSString *) title :(NSString *) body :(NSString *) image;
{
    NSUserNotification *notification = [[NSUserNotification alloc] init];
    notification.title = title;
    notification.subtitle = body;
    //notification.contentImage = [NSImage imageNamed:image];
    [notification set_identityImage:[NSImage imageNamed:image]];
    [NSUserNotificationCenter.defaultUserNotificationCenter deliverNotification:notification];
}

- (NSString *) getInput :(NSString *) prompt
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
        return @"";
    } else {
        NSAssert1(NO, @"Invalid input dialog button %d", button);
        return @"";
    }
}

- (void) markNotificationsRead
{
    self.bridge->markNotificationsRead();
}

- (void) exit
{
    [[NSApplication sharedApplication] terminate:nil];
}

@end

void AppDelegateBridge::setBridge(AppDelegateBridgeNative *bridge)
{
    this->bridge = bridge;
}

void AppDelegateBridge::notify(std::string title, std::string body, std::string image)
{
    [bridge notify:[NSString stringWithUTF8String:title.c_str()] :[NSString stringWithUTF8String:body.c_str()] :[NSString stringWithUTF8String:image.c_str()]];
}

void AppDelegateBridge::setNotificationCount(int count)
{
    [bridge setNotificationCount:count];
}

std::string AppDelegateBridge::getInput(std::string prompt)
{
    return [[bridge getInput:[NSString stringWithUTF8String:prompt.c_str()]] UTF8String];
}

void AppDelegateBridge::markNotificationsRead()
{
    std::cout<<"BLAH!";
}