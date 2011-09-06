//
//  msend_guiAppDelegate.h
//  msend-gui
//
//  Created by Martin Knoll on 03.09.11.
//  Copyright 2011 Otto-von-Guericke-Universität Magdeburg. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface msend_guiAppDelegate : NSObject <NSApplicationDelegate> {
    NSFileHandle *listening_handle;
    
    NSWindow *window;
    NSTextField *receiver_textfield;
    NSTextField *port_textfield;
    NSPathControl *path_pathcontrol;
    NSTextField *log_textfield;
    NSButton *receive_button;
}

@property (assign) IBOutlet NSWindow *window;

@property (assign) IBOutlet NSTextField *receiver_textfield;

@property (assign) IBOutlet NSTextField *port_textfield;

@property (assign) IBOutlet NSPathControl *path_pathcontrol;

@property (assign) IBOutlet NSTextField *log_textfield;

@property (assign) IBOutlet NSButton *receive_button;

- (IBAction)click_receive:(id)sender;

@end
