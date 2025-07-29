//
//  detectkbds.h
//  67a1ca5c-80b0-8012-8c77-855a5312b8b1
//
//  Created by Bilal Ahsan on 12/04/2025.
//

#ifndef DETECTKBDS_H
#define DETECTKBDS_H

#include <ApplicationServices/ApplicationServices.h>
#include <IOKit/hid/IOHIDManager.h>
#include <CoreFoundation/CoreFoundation.h>
#include <Carbon/Carbon.h>
#include <stdio.h>

#include <vector>
#include <utility>

#define TARGET_VENDOR_ID 0x05AC  // Example: Apple Inc.
#define TARGET_PRODUCT_ID 0x0250 // Example: Specific Apple keyboard

#define VENDOR_ID_APPLE 0x05AC  // Example: Apple Inc.
#define PRODUCT_ID_MAC_USB_KEYBOARD_LARGE 0x0250 // Example: Specific Apple keyboard


// Stores detected keyboard Vendor & Product IDs
typedef struct {
    int vendorID;
    int productID;
    IOHIDDeviceRef device;
} KeyboardInfo;

// read these variables for results
extern KeyboardInfo detectedKeyboards[10]; // Store up to 10 keyboards
extern int keyboardCount;
extern std::vector<std::pair<int, int>> keyboards;

void detectKeyboards();

#endif
