#import <Cocoa/Cocoa.h>
#import <ApplicationServices/ApplicationServices.h>
#include <string>
#include <thread>
#include <chrono>
#include <iostream>

// Get frontmost app PID using NSWorkspace
pid_t getFrontmostAppPID() {
    NSRunningApplication *frontApp = [[NSWorkspace sharedWorkspace] frontmostApplication];
    return frontApp.processIdentifier;
}

// Activate app by PID using Accessibility API
bool activateApp(pid_t pid) {
    AXUIElementRef app = AXUIElementCreateApplication(pid);
    if (!app) return false;

    AXError err = AXUIElementPerformAction(app, kAXRaiseAction);
    CFRelease(app);
    return (err == kAXErrorSuccess);
}

// Send typed characters via CGEvents
void typeString(const std::string& text) {
    for (char c : text) {
        CGEventRef keyDown = CGEventCreateKeyboardEvent(NULL, 0, true);
        CGEventRef keyUp = CGEventCreateKeyboardEvent(NULL, 0, false);
        UniChar unicodeChar = c;
        CGEventKeyboardSetUnicodeString(keyDown, 1, &unicodeChar);
        CGEventKeyboardSetUnicodeString(keyUp, 1, &unicodeChar);
        CGEventPost(kCGHIDEventTap, keyDown);
        CGEventPost(kCGHIDEventTap, keyUp);
        CFRelease(keyDown);
        CFRelease(keyUp);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}



// Function to get window titles from all visible apps
std::vector<std::pair<std::string, std::string>> getAllWindowTitles() {
    std::vector<std::pair<std::string, std::string>> results;

    // Get the list of all windows on the screen
    CFArrayRef windowList = CGWindowListCopyWindowInfo(kCGWindowListOptionOnScreenOnly, kCGNullWindowID);
    if (!windowList) return results;

    CFIndex count = CFArrayGetCount(windowList);

    // Iterate through each window and get the title and associated application name
    for (CFIndex i = 0; i < count; ++i) {
        CFDictionaryRef dict = (CFDictionaryRef)CFArrayGetValueAtIndex(windowList, i);
        CFStringRef windowTitle = (CFStringRef)CFDictionaryGetValue(dict, kCGWindowName);
        if (!windowTitle) continue;

        char buffer[1024];
        CFStringGetCString(windowTitle, buffer, sizeof(buffer), kCFStringEncodingUTF8);

        // Get the application name (owner) of the window
        CFNumberRef pidNum = (CFNumberRef)CFDictionaryGetValue(dict, kCGWindowOwnerPID);
        pid_t pid = 0;
        CFNumberGetValue(pidNum, kCFNumberIntType, &pid);

        // Get the app name by its PID
        ProcessSerialNumber psn;
        if (GetProcessForPID(pid, &psn) == noErr) {
            CFStringRef appNameRef = nullptr;
            if (CopyProcessName(&psn, &appNameRef) == noErr) {
                char appName[1024];
                CFStringGetCString(appNameRef, appName, sizeof(appName), kCFStringEncodingUTF8);
                results.emplace_back(appName, buffer);  // Store app name and window title
                CFRelease(appNameRef);
            }
        }
    }

    CFRelease(windowList);
    return results;
}

// Locate window by title and send key sequence
void sendKeysToWindowByTitle(const std::string& title, const std::string& keys) {
    pid_t originalPID = getFrontmostAppPID();

    CFArrayRef windowList = CGWindowListCopyWindowInfo(kCGWindowListOptionOnScreenOnly, kCGNullWindowID);
    if (!windowList) return;

    pid_t targetPID = 0;
    bool found = false;
    CFIndex count = CFArrayGetCount(windowList);

    for (CFIndex i = 0; i < count; ++i) {
        CFDictionaryRef dict = (CFDictionaryRef)CFArrayGetValueAtIndex(windowList, i);
        CFStringRef windowTitle = (CFStringRef)CFDictionaryGetValue(dict, kCGWindowName);
        if (!windowTitle) continue;

        char buffer[1024];
        CFStringGetCString(windowTitle, buffer, sizeof(buffer), kCFStringEncodingUTF8);

        if (title == buffer) {
            CFNumberRef pidNum = (CFNumberRef)CFDictionaryGetValue(dict, kCGWindowOwnerPID);
            CFNumberGetValue(pidNum, kCFNumberIntType, &targetPID);
            found = true;
            break;
        }
    }

    CFRelease(windowList);

    if (!found) {
        std::cerr << "Window with title \"" << title << "\" not found.\n";
        return;
    }

    if (!activateApp(targetPID)) {
        std::cerr << "Failed to activate app with PID: " << targetPID << "\n";
        return;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    typeString(keys);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    activateApp(originalPID);
}

// Main function with command-line arguments
int pain(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " \"Window Title\" \"Key Sequence\"\n";
        return 1;
    }

    std::string windowTitle = argv[1];
    std::string keySequence = argv[2];
    sendKeysToWindowByTitle(windowTitle, keySequence);
    return 0;
}



void sendKeystrokesToApp_carbon_way(pid_t pid, const std::string& keys) {
    // Get the target application's process serial number
    ProcessSerialNumber psn;
    if (GetProcessForPID(pid, &psn) != noErr) {
        // Handle error
        return;
    }

    // Create keyboard events
    for (char c : keys) {
        // Create key down event
        CGEventRef keyDown = CGEventCreateKeyboardEvent(nullptr, 0, true);
        CGEventSetIntegerValueField(keyDown, kCGKeyboardEventKeycode, c);
        CGEventPostToPSN(&psn, keyDown);
        CFRelease(keyDown);
        
        // Create key up event
        CGEventRef keyUp = CGEventCreateKeyboardEvent(nullptr, 0, false);
        CGEventSetIntegerValueField(keyUp, kCGKeyboardEventKeycode, c);
        CGEventPostToPSN(&psn, keyUp);
        CFRelease(keyUp);
    }
}


void sendKeystrokesToApp_accessibility_way(pid_t pid, const std::string& keys) {
    AXUIElementRef app = AXUIElementCreateApplication(pid);
    
    if (app) {
        // Focus the target application's main window
        AXUIElementRef window = nullptr;
        AXUIElementCopyAttributeValue(app, kAXMainWindowAttribute, (CFTypeRef *)&window);
        
        if (window) {
            // Set the element as the focused UI element
            AXUIElementSetAttributeValue(window, kAXFocusedAttribute, kCFBooleanTrue);
            
            // Send keystrokes
            for (char c : keys) {
                CGEventRef keyDown = CGEventCreateKeyboardEvent(nullptr, 0, true);
                CGEventSetIntegerValueField(keyDown, kCGKeyboardEventKeycode, c);
                AXUIElementPostKeyboardEvent(window, 0, c, true);
                CFRelease(keyDown);
                
                CGEventRef keyUp = CGEventCreateKeyboardEvent(nullptr, 0, false);
                CGEventSetIntegerValueField(keyUp, kCGKeyboardEventKeycode, c);
                AXUIElementPostKeyboardEvent(window, 0, c, false);
                CFRelease(keyUp);
            }
            
            CFRelease(window);
        }
        CFRelease(app);
    }
}

int main(int argc, char* argv[]) {

sendKeystrokesToApp_carbon_way(1719,"hello");
sendKeystrokesToApp_accessibility_way(1719, "hello");
return 0;

    auto windows = getAllWindowTitles();
    for (const auto& [app, title] : windows) {
        std::cout << app << " - " << title << "\n";
    }
    return 0;
    return pain(argc, argv);
}
