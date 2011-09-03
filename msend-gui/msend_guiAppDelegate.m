//
//  msend_guiAppDelegate.m
//  msend-gui
//
//  Created by Martin Knoll on 03.09.11.
//  Copyright 2011 Otto-von-Guericke-Universität Magdeburg. All rights reserved.
//

#import "msend_guiAppDelegate.h"
#import "send.h"

@implementation msend_guiAppDelegate

@synthesize window;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    // Insert code here to initialize your application
}

- (IBAction)click_test:(id)sender {
    send_to("localhost", "8080", "/Users/martin/dev/msend/Test");
}

@end
