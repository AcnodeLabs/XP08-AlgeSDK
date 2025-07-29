//
//  objc_code.mm
//  AlgeApp
//
//  Created by Bilal Ahsan on 21/04/2025.
//

#include <iostream>
#import <ScriptingBridge/ScriptingBridge.h>
#import "SystemEvents.h"


int test_systemevent_h_integration(const char* appTitleOrBundleID, const char* textToSend) {
    @autoreleasepool {
        NSString *target = [NSString stringWithUTF8String:appTitleOrBundleID];
        NSString *text = [NSString stringWithUTF8String:textToSend];

        // Find running apps
        NSArray<NSRunningApplication *> *runningApps = [[NSWorkspace sharedWorkspace] runningApplications];
        NSRunningApplication *targetApp = nil;

        for (NSRunningApplication *app in runningApps) {
            if ([app.localizedName isEqualToString:target] || [app.bundleIdentifier isEqualToString:target]) {
                targetApp = app;
                break;
            }
        }

        if (!targetApp) {
            std::cerr << "Could not find app with title or bundle ID: " << appTitleOrBundleID << "\n";
            return 1;
        }

        // Bring app to front
        [targetApp activateWithOptions:NSApplicationActivateIgnoringOtherApps];

        // Wait briefly to ensure focus change
        [NSThread sleepForTimeInterval:5];

        // Now send keystroke via System Events
        SystemEventsApplication *systemEvents = [SBApplication applicationWithBundleIdentifier:@"com.apple.systemevents"];

        //- (void) keystroke:(NSString *)x using:(id)using_;  // cause the target process to behave as if keystrokes were entered
        //[systemEvents keystroke:text];
        [systemEvents activate];
        
        
        [systemEvents keyCode:0 using:nil];

        
        std::cout << "Keystroke sent to app: " << appTitleOrBundleID << "\n";
        return 0;
    }
}


int z_old(const char* textToSend) {
    @autoreleasepool {
        // Create System Events app object
        SystemEventsApplication *systemEvents = [SBApplication applicationWithBundleIdentifier:@"com.apple.systemevents"];

        // Convert C-string to NSString
        NSString *text = [NSString stringWithUTF8String:textToSend];

        // Send keystroke
        [systemEvents keystroke:text];

        std::cout << "Keystroke sent: " << textToSend << "\n";
    }

    return 0;
}
