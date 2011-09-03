//
//  msend_guiAppDelegate.m
//  msend-gui
//
//  Created by Martin Knoll on 03.09.11.
//  Copyright 2011 Otto-von-Guericke-Universität Magdeburg. All rights reserved.
//

#import "msend_guiAppDelegate.h"
#import "send.h"
#import "receive.h"
#import <sys/socket.h>
#import <pthread.h>

@implementation msend_guiAppDelegate

@synthesize receiver_textfield;
@synthesize port_textfield;
@synthesize path_pathcontrol;
@synthesize log_textfield;
@synthesize receive_button;
@synthesize window;

pthread_t th;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    // Insert code here to initialize your application
}


- (IBAction)click_receive:(id)sender 
{
    if ([receive_button state] == NSOnState) {
        [log_textfield setStringValue:@"An"];
        pthread_create(&th, NULL, (void *)receive_from, "8080");
    }
    else {
        [log_textfield setStringValue:@"Aus"];
        pthread_exit(th); //doesnt work
    }
}
@end
