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

- (void) userNotificationCenter:(NSUserNotificationCenter *)center didActivateNotification:(NSUserNotification *)notification
{
    printf("NOTIFICATION!\n");
}

- (AppDelegateBridgeNative *) init
{
    if (self = [super init]) {
        [[NSUserNotificationCenter defaultUserNotificationCenter] setDelegate:self];
    
        self.statusBar = [[NSStatusBar systemStatusBar] statusItemWithLength:NSSquareStatusItemLength];
        self.statusBar.image = [NSImage imageNamed:@"notification_light"];
        self.statusBar.alternateImage =[NSImage imageNamed:@"notification_alt"];
        self.statusBar.highlightMode = YES;
        
        self.menuZone = [NSMenu menuZone];
        self.menu = [[NSMenu allocWithZone:self.menuZone] init];
        [self.menu setAutoenablesItems:NO];
    
        self.markNotificationsReadMenuItem = [self.menu addItemWithTitle:@"Mark all as read"
                                    action:@selector(markNotificationsRead)
                                    keyEquivalent:@""];
        self.markNotificationsReadMenuItem.enabled = false;
        [self.markNotificationsReadMenuItem setTarget:self];
    
        [self.menu addItem:[NSMenuItem separatorItem]];
        
        self.exitMenuItem = [self.menu addItemWithTitle:@"Exit"
                                                 action:@selector(exit)
                                          keyEquivalent:@""];
        [self.exitMenuItem setTarget:self];
        
        self.settingsMenuItem = [self.menu addItemWithTitle:@"Settings"
                                                 action:@selector(settings)
                                          keyEquivalent:@""];
        [self.settingsMenuItem setTarget:self];
    
        self.statusBar.menu = self.menu;
    }
    
    return self;
}

- (void) setNotificationCount :(int) count
{
    if (count > 0) {
        self.statusBar.image = [NSImage imageNamed:@"notification_dark"];
        self.markNotificationsReadMenuItem.enabled = YES;
    } else {
        self.statusBar.image = [NSImage imageNamed:@"notification_light"];
        self.markNotificationsReadMenuItem.enabled = NO;
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

- (void) getInput :(NSString *) prompt :(NSString **) r
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
        *r = [input stringValue];
    } else if (button == NSAlertAlternateReturn) {
        *r = @"";
    } else {
        NSAssert1(NO, @"Invalid input dialog button %d", button);
        *r = @"";
    }
}

- (void) alert :(NSString *) prompt
{
    NSAlert *alert = [NSAlert alertWithMessageText: prompt
                        defaultButton:@"OK"
                                   alternateButton:nil
                                       otherButton:nil
                         informativeTextWithFormat:@""];
    
    [alert runModal];
}

- (void) markNotificationsRead
{
    self.bridge->event("markNotificationsRead");
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
    __block NSString *r;
    
    dispatch_sync(dispatch_get_main_queue(), ^{
        [bridge getInput :[NSString stringWithUTF8String:prompt.c_str()] :&r];
    });
    
    return [r UTF8String];
}

void AppDelegateBridge::alert(std::string prompt)
{
    [bridge performSelectorOnMainThread:@selector(alert:) withObject:[NSString stringWithUTF8String:prompt.c_str()] waitUntilDone:NO];
}

void AppDelegateBridge::addEvent(std::string eventName, EventCallback *callback)
{
    callbacks[eventName] = callback;
}

void AppDelegateBridge::removeEvent(std::string eventName)
{
    callbacks[eventName] = NULL;
}

void AppDelegateBridge::event(std::string eventName, void *data)
{
    std::thread t(callbacks[eventName], data);
    t.join();
}