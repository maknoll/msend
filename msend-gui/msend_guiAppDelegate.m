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

@implementation msend_guiAppDelegate

@synthesize receiver_textfield;
@synthesize port_textfield;
@synthesize path_pathcontrol;
@synthesize log_textfield;
@synthesize receive_button;
@synthesize window;

- (void)start_receiving
{
    int sock = socket_bind_listen("8080");
    
    listening_handle = [[NSFileHandle alloc]
                       initWithFileDescriptor:sock
                       closeOnDealloc:YES];
    
    [[NSNotificationCenter defaultCenter] 
     addObserver:self
        selector:@selector(accept_client:)
            name:NSFileHandleConnectionAcceptedNotification
          object:nil];
    [listening_handle acceptConnectionInBackgroundAndNotify];
}

- (void)stop_receiving
{
    [listening_handle dealloc];
}

- (void)accept_client:(NSNotification *)notfication
{
    NSFileHandle *client = [[notfication userInfo] objectForKey:NSFileHandleNotificationFileHandleItem];
    receive_file([client fileDescriptor]);
    close([client fileDescriptor]);
    
    [listening_handle acceptConnectionInBackgroundAndNotify];
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    // Insert code here to initialize your application
}


- (IBAction)click_receive:(id)sender 
{
    if ([receive_button state] == NSOnState) {
        [log_textfield setStringValue:@"An"];
        [self start_receiving];
    }
    else {
        [log_textfield setStringValue:@"Aus"];
        [self stop_receiving];
    }
}
@end
