//
//  detectkbds.cpp
//  67a1ca5c-80b0-8012-8c77-855a5312b8b1
//
//  Created by Bilal Ahsan on 12/04/2025.
//




#include "detectkbds.h"
#include <vector>
#include <utility>

KeyboardInfo detectedKeyboards[10]; // Store up to 10 keyboards
int keyboardCount = 0;

std::vector<std::pair<int, int>> keyboards;
// Add some pairs

// Callback function for HID device detection
void HIDKeyboardCallback(void *context, IOReturn result, void *sender, IOHIDDeviceRef device) {
    CFTypeRef vendorIDRef = IOHIDDeviceGetProperty(device, CFSTR(kIOHIDVendorIDKey));
    CFTypeRef productIDRef = IOHIDDeviceGetProperty(device, CFSTR(kIOHIDProductIDKey));

    if (vendorIDRef && productIDRef) {
        
        int vendorID = 0, productID = 0;
        CFNumberGetValue((CFNumberRef)vendorIDRef, kCFNumberIntType, &vendorID);
        CFNumberGetValue((CFNumberRef)productIDRef, kCFNumberIntType, &productID);
        printf("Detected Keyboard - Vendor ID: 0x%X, Product ID: 0x%X\n", vendorID, productID);
        
        if (keyboardCount < 10) {
            detectedKeyboards[keyboardCount].vendorID = vendorID;
            detectedKeyboards[keyboardCount].productID = productID;
            detectedKeyboards[keyboardCount].device = device;

            keyboards.emplace_back(vendorID, productID);

            keyboardCount++;
            
        }
    }
}



/*
 Synergy:::Detected Keyboard - Vendor ID: 0x4E53, Product ID: 0x5407
 MacSerialKbgLarge:: Detected Keyboard - Vendor ID: 0x5AC, Product ID: 0x250
 */

void detectKeyboards() {
    
    IOHIDManagerRef hidManager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
     if (!hidManager) {
         printf("Failed to create HID Manager.\n");
         return;
     }

     // Filter for keyboard devices
     CFMutableDictionaryRef matchingDict = CFDictionaryCreateMutable(kCFAllocatorDefault, 0,
                                                                     &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
     int usagePage = kHIDPage_GenericDesktop;
     int usage = kHIDUsage_GD_Keyboard;
     CFNumberRef pageRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &usagePage);
     CFNumberRef usageRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &usage);

     CFDictionarySetValue(matchingDict, CFSTR(kIOHIDDeviceUsagePageKey), pageRef);
     CFDictionarySetValue(matchingDict, CFSTR(kIOHIDDeviceUsageKey), usageRef);

     IOHIDManagerSetDeviceMatching(hidManager, matchingDict);
     IOHIDManagerRegisterDeviceMatchingCallback(hidManager, HIDKeyboardCallback, NULL);
     IOHIDManagerScheduleWithRunLoop(hidManager, CFRunLoopGetCurrent(), kCFRunLoopCommonModes);
     IOHIDManagerOpen(hidManager, kIOHIDOptionsTypeNone);

     CFRunLoopRunInMode(kCFRunLoopDefaultMode, 1, false); // Run for a short time to detect devices

     CFRelease(pageRef);
     CFRelease(usageRef);
     CFRelease(matchingDict);
 }



