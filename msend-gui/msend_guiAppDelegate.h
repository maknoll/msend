//
//  msend_guiAppDelegate.h
//  msend-gui
//
//  Created by Martin Knoll on 03.09.11.
//  Copyright 2011 Otto-von-Guericke-Universität Magdeburg. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface msend_guiAppDelegate : NSObject <NSApplicationDelegate> {
    NSWindow *window;
}

- (IBAction)click_test:(id)sender;

@property (assign) IBOutlet NSWindow *window;

@end
