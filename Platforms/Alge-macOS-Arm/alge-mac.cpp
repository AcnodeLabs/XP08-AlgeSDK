//
//  alge-mac.cpp
//  Alge-Mac
//
//  Created by Bilal on 6/20/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
// http://scrumy.com/XAL_AlgeSDK
// M1 Edit

#define MACOSX
#define XCODE_BUILD
//#define NO_BOX2D
#define NO_NATS

#define M1ARM

#ifdef M1ARM
#define NO_FONTLIB
#endif

//see https://developer.apple.com/metal/cpp/
#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>
#include <Carbon/Carbon.h>
///

#include "xgui.hpp"
#define glOrthof glOrtho
#include "../../SDKSRC/Base/CBaseV1_2.h"
#include "AlgeApp-Bridging-Header.h"

/*
#ifndef WINDOWS
class GuiFont : public ImFont {
    ImFont* thiz;
public:
    GuiFont() {
        
    }

    void Use(std::string fontfile, float size) {
        thiz = ImGui::GetIO().Fonts->AddFontFromFileTTF(fontfile.c_str(), size * m_fontScale);
    }

    void Use(std::string font) {

    }

};
GuiFont imguiFont;
#endif
*/

int NatsServiceRequest(string svc, string url) {
#ifndef NO_NATS
    static int httpReuestId = 10000000;
    netmsg.Post("xal.e."+svc, std::to_string(httpReuestId) + "|" + url);
    httpReuestId++;
    return httpReuestId - 1;
#endif
}

#include "CANDIDATE.h"

#ifndef NO_FMOD
   #include "../../SDKSRC/Base/fmod/framework.hpp"
#endif

#include "spng/spng.h"

class XPng {
    char png_buf[1024*4];
    size_t png_buf_size = 4096;
    size_t out_size = 4096;
    unsigned char *out = NULL;
public:
    void Init() {

    }
};



#include <GLUT/glut.h>

#include <stdio.h>
#include <iostream>
#include <future>
#include <chrono>
#include <cstdlib>

XHttpSocket msck;

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

int kWindowWidth =	(1920);
int kWindowHeight = (1080);
//#include CANDIDATE

#define TIMERMSECS 33


int startTime;
int prevTime;

#ifndef NO_FONTLIB
struct dtx_font *font;
#endif

void alPrintText(string text, float scale) {
#ifndef NO_FONTLIB
    glPushMatrix();
    glScalef(scale, scale, scale);
    
    if (font!=nullptr) dtx_string(text.c_str());
 glPopMatrix();
#endif
}

App app;

extern void(*callBackIn)(string);
#ifndef NO_NATS
CNetMsg netmsg;
#endif // !


static int elapsedTime;

#include "Timer.h"


Timer time1;
float deltaT;
float lastTime;
float aX, aY, aZ;


#include "detectkbds.h"
void CreateKeyboardEventTap(); //forward decl
CFMachPortRef eventTap;
int setup_handle_HID_input();
IOHIDManagerRef hidManager;
int sendkeystroketoapptitled(std::string);
/*
 
 #include "MyOpenALSupport.h"
 
ALsizei nAL;
#define NUM_BUFFERS_SOURCES 16
ALuint alSources[NUM_BUFFERS_SOURCES];
ALuint alBuffers[NUM_BUFFERS_SOURCES];
float alPosition[3];

void initAL(char* respath) {
    nAL = NUM_BUFFERS_SOURCES;
    alGetError();
    
    ALCcontext		*newContext = NULL;
	ALCdevice		*newDevice = NULL;
    
	newDevice = alcOpenDevice(NULL);
	if (newDevice != NULL)
	{
		newContext = alcCreateContext(newDevice, 0);
		if (newContext != NULL)
		{
			alcMakeContextCurrent(newContext);
            
        }
    }
    alListenerfv(AL_POSITION, alPosition);
    alGenBuffers(nAL, alBuffers);
    alGenSources(nAL, alSources);
    

    
}

void deInitAL() {
 ALCcontext	*context = NULL;
 ALCdevice	*device = NULL;
 ALuint		returnedNames[NUM_BUFFERS_SOURCES];

 alDeleteSources(NUM_BUFFERS_SOURCES, returnedNames);
 alDeleteBuffers(NUM_BUFFERS_SOURCES, returnedNames);
 context = alcGetCurrentContext();
 device = alcGetContextsDevice(context);
 alcDestroyContext(context);


    
alcCloseDevice(device);
    
    
    
}

void list_audio_devices(const ALCchar *devices)
{

}

void sndSet(char* filename, int id, int loops) {
 ALenum  format;
 ALvoid* data;
 ALsizei size;
 ALsizei freq;
	
 if (loops>0) alSourcei(alSources[id], AL_LOOPING, AL_TRUE);
 alSourcefv(alSources[id], AL_POSITION, alPosition);
 alSourcef(alSources[id],AL_REFERENCE_DISTANCE, 0.0f);
 
 char filespec[256];
 sprintf(filespec, "%s/%s", app.rm.resourcepath, filename);
 
 CFStringRef f1 = CFStringCreateWithFileSystemRepresentation(kCFAllocatorDefault, filespec);
 CFURLRef fileURL = CFURLCreateWithString(kCFAllocatorDefault, (CFStringRef)f1, NULL);
    data = malloc(1);//MyGetOpenALAudioData(fileURL, &size, &format, &freq);
 CFRelease(fileURL);
 alBufferData(alBuffers[id], format, data, size, freq);
 free(data);
 alSourcei(alSources[id], AL_BUFFER, alBuffers[id]);
}

void sndPlay(int id) {
    printf("Trying to Play Sound# %d", id);
    alSourcePlay(alSources[id]);
}
*/

extern void ShowToast(char*);

//c++ macOS send keystrokes via pid
//https://chatgpt.com/c/68008858-076c-8012-9af6-5b52f180db1a

// Function to send a keypress (for example: 'A')
void sendkeystroke_sendKeyPress(CGKeyCode keyCode) {
    CGEventRef keyDown = CGEventCreateKeyboardEvent(NULL, keyCode, true);
    CGEventRef keyUp = CGEventCreateKeyboardEvent(NULL, keyCode, false);

    CGEventPost(kCGHIDEventTap, keyDown);
    CGEventPost(kCGHIDEventTap, keyUp);

    CFRelease(keyDown);
    CFRelease(keyUp);
}

// Function to make application with given PID frontmost
bool sendkeystroke_makeAppFrontmost(pid_t pid) {
    AXUIElementRef app = AXUIElementCreateApplication(pid);
    AXError err = AXUIElementPerformAction(app, kAXRaiseAction);
    CFRelease(app);
    return (err == kAXErrorSuccess);
}


int runCommandWithTimeout(const std::string& command, int timeoutSeconds) {
    auto future = std::async(std::launch::async, [&command]() -> int {
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));  // Delay before executing
        return system(command.c_str());
    });

    // Wait for the result with timeout
    if (future.wait_for(std::chrono::seconds(timeoutSeconds)) == std::future_status::ready) {
        return future.get();  // Command finished successfully
    } else {
        std::cerr << "Command timed out: " << command << std::endl;
        // ⚠️ system() can't be easily killed. This only tells you it's hanging.
        // Consider using `CreateProcess()` on Windows or `fork()` + `exec()` on Unix for fine control.
        return -1;  // You can define a custom error code
    }
}

int keystrokeSendAttempts = 0;
extern int test_systemevent_h_integration(const char* appTitle, const char* textToSend);

void sendkeystroke_main_systemevents_h_way(const char* appTitle, const char* keystroke) {
    test_systemevent_h_integration(appTitle, keystroke);
}

void sendkeystroke_main_applescript_way(const char* apptitle, const char* keystroke) {
    
    
    // Construct the AppleScript command
    char scriptCommand[512] = {0};
    snprintf(scriptCommand, sizeof(scriptCommand), "/usr/bin/osascript /Users/aliveb/REPOS/XP08-AlgeSDK/Platforms/Alge-macOS-Arm/send-key-and-restore.applescript \"%s\" \"%s\"", apptitle, keystroke);
    //printf(scriptCommand, sizeof(scriptCommand), "/usr/bin/osascript /Users/aliveb/REPOS/XP08-AlgeSDK/Platforms/Alge-macOS-Arm/send-key-and-restore.applescript \"%s\" \"%s\"", apptitle, keystroke);
    snprintf(scriptCommand, sizeof(scriptCommand), "python3 /Users/aliveb/REPOS/XP08-AlgeSDK/Platforms/Alge-macOS-Arm/testKey2.py \"%s\" \"%s\"");
    
    // Run the command
   // int result = system(scriptCommand);
    keystrokeSendAttempts++;
    int result = runCommandWithTimeout(string(scriptCommand), 3);
    // Print result
    if (result == 0) {
        printf("AppleScript call#%d suceeded with code %d\n", keystrokeSendAttempts,result);
    } else {
        printf("AppleScript call#%d failed with code %d\n", keystrokeSendAttempts,result);
    }
}

int sendkeystroke_main_embedded_way(pid_t targetPID) {
     
    // Raise the app (bring to front)
    if (sendkeystroke_makeAppFrontmost(targetPID)) {
        std::cout << "App raised, sending keystroke in 2 seconds..." << std::endl;
        sleep(2);
        sendkeystroke_sendKeyPress((CGKeyCode)0); // keyCode 0 = 'A' on US keyboard
    } else {
        std::cerr << "Failed to bring app frontmost or not allowed.\n";
    }

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


int sendkeystroke_main(const char* t, const char* k) {
    sendkeystroke_main_systemevents_h_way(t, k);
    //sendkeystroke_main_applescript_way(t, k);
    //sendKeystrokesToApp_carbon_way(1719,"hello");
    //sendKeystrokesToApp_accessibility_way(1719, "hello");
    return 0;
}


//~ 68008858-076c-8012-9af6-5b52f180db1a


static void exitIt() {
  app.Deinit();
#ifndef NO_FMOD
  FMOD_Deinit();
#else
    deInitAL();
#endif
  exit(0);
}

void processOutput() {
 PEG::CMD* c = app.output.pull();
 switch (c->command) {
   case CMD_SNDSET0:
   case CMD_SNDSET1:
   case CMD_SNDSET2:
   case CMD_SNDSET3:
        {
       
            string fullpath = string(app.rm.resourcepath) + string("/")+ string((char*)c->param1);
#ifndef NO_FMOD
            FMOD_Set(c->command-CMD_SNDSET, (char*)fullpath.c_str());
#else
            sndSet((char*)c->param1, c->command-CMD_SNDSET, c->i2);
#endif
        }
   break;
  
     case CMD_SNDPLAY0:
     case CMD_SNDPLAY1:
     case CMD_SNDPLAY2:
     case CMD_SNDPLAY3:
            {
               
#ifndef NO_FMOD
                FMOD_Play(c->command-CMD_SNDPLAY);
#else
                sndPlay((c->command) - (CMD_SNDPLAY));
#endif
            }
   break;
        case CMD_TITLE:
         glutSetWindowTitle( (char*)c->param1);
        break;
            
        case CMD_TOAST:
         ShowToast((char*)c->param1);
        break;
         
        case CMD_SENDKEYTO_APP: {
            static const char* appTitle = (const char*)c->param1;
            static const char* keyStrokes = (const char*)c->param2;
            sendkeystroke_main(appTitle, keyStrokes);
        }
        break;
         
        case KEYEVENT_TAP:
            if (!eventTap) {
                CreateKeyboardEventTap();
            }
         break;
     
        case HID_INPUT:
            setup_handle_HID_input();
         break;
         
     case CMD_END:
         CFRelease(hidManager);
         exitIt();
     break;
    }
}

static void HandleDisplay( )
{
 float newTime = time1.getElapsedTimeInSec();
 deltaT =  newTime - lastTime;
 if (aX==0 && aY==0 && aZ==0) aY=-9.8*100;
    app.Render(deltaT,aX,aY,aZ);

   processOutput();
   glutSwapBuffers( );
   
 lastTime = newTime;
}

static void animate(int value)
{
 glutTimerFunc(TIMERMSECS, animate, 0);
 int currTime = glutGet(GLUT_ELAPSED_TIME);
 int timeSincePrevFrame = currTime - prevTime;
 elapsedTime = currTime - startTime;
 glutPostRedisplay();
 prevTime = currTime;


}

static int i=0;
static void HandleIdle() {
 glutPostRedisplay();
 CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0.001, false); // Run for a short time to detect devices
}

void appSize(int w, int h, int scale) {
    app.coord.setResolutionReported(i2(w,h), scale);
    app.input.pushI(CMD_SCREENSIZE, app.coord.getResolutionForGameEngine().x, app.coord.getResolutionForGameEngine().y);
 //   app.CamReshape(i2(app.coord.getResolutionForGameEngine().x, app.coord.getResolutionForGameEngine().y));
}

static int reshapeCounter = 0;//as per current coding this first func call seems redundant so it is suppressed in line 215
static void HandleReshape( const int ewidth, const int eheight )
{
    reshapeCounter++;
    int width = ewidth ;
    int height = eheight;
    
    if (eheight>0 && ewidth!=eheight)										// Prevent A Divide By Zero By
    {
      glViewport(0,0,width,height);						// Commenting it for fixed width
      ImGui_ImplGLUT_ReshapeFunc(width, height);
      if (reshapeCounter==1) return;//as per current coding this first func call seems redundant so it is suppressed in line 215
      printf("[%dx%d]", width, height);      
      appSize(width, height, 1);
      glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
      glLoadIdentity();									// Reset The Projection Matrix
      glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
      glLoadIdentity();
    }
}



static void HandleKeyBoardUp( unsigned char key, int x, int y)
{
  app.input.pushI(CMD_KEYUP, key, 0);
}

static void HandleKeyBoard( const unsigned char inKey, const int inPosX, const int inPosY )
{
    ImGui_ImplGLUT_KeyboardFunc(inKey,inPosX,inPosY);
  if (inKey==27) {exitIt();}
  app.input.pushI(CMD_KEYDOWN, inKey, 0);
}

char msgg[128];

static void HandleSpecialKey( const int inKey, const int inPosX, const int inPosY )
{
    ImGui_ImplGLUT_SpecialFunc(inKey, inPosX, inPosY);
    if (inKey==27) {exitIt();}
    app.input.pushI(CMD_KEYDOWN, inKey, 0);
}

static void HandleMouse( const int inButton, const int inState, const int inPosX, const int inPosY )
{
  if (inButton==0) app.input.pushI(inState==0?CMD_TOUCH_START:CMD_TOUCH_END, inPosX, inPosY);
  ImGui_ImplGLUT_MouseFunc(inButton,inState,inPosX,inPosY);
}

static void HandleMotion( const int inPosX, const int inPosY )
{
  int x = inPosX;
  int y = inPosY;
  if (x!=0 || y!=0)
      app.input.pushI(CMD_TOUCHMOVE, x, y);
    ImGui_ImplGLUT_MotionFunc(inPosX,inPosY);
    printf(msgg, "ImGui_ImplGLUT_MotionFunc %d%d", inPosX, inPosY);
}

char ResPath[MAX_PATH];

void FindAppName() {
  FILE* f = fopen("CANDIDATE.h", "r");
  if (f) {
    char line[256];
    for (int i=0; i<100; i++) {
	 fscanf(f, "%s", line);
	 char* pn = strstr(line, ".hpp");
	 strcpy(ResPath, line);
	
    if (pn) {
	 int l = int(strlen(ResPath));
	  ResPath[l-8]=0;
  	  strcat(ResPath, "/Data");
	   break;
    }
	}
	fclose(f);
	}
}
void reset_window_title(int para)
{
 //   glutSetWindowTitle(APP_TITLE);
 //   glutFullScreen();
    
    glutReshapeWindow(app.coord.getResolutionForGameEngine().x, app.coord.getResolutionForGameEngine().y);
  //  glutPositionWindow(0,0);
  //  HandleReshape(kWindowWidth, kWindowHeight);
}
#include <CoreGraphics/CGDisplayConfiguration.h>


static char remoteCommand[256];

void onRemoteCommand(string cmd) {
    strcpy(remoteCommand, cmd.c_str());
    app.input.pushP(CMD_REMOTE_COMMAND, (void*)remoteCommand, 0);
}

char commandline[256] = {0,0};
using namespace MTL;

void MetalStuff() {

    Device* _device = CreateSystemDefaultDevice();
    Library* lib = _device->newDefaultLibrary();

    
 //   r->setLabel("Simple Pipeline");
    
    _device->release();
}

int getHIDIntProperty(IOHIDDeviceRef device, CFStringRef key) {
    CFTypeRef property = IOHIDDeviceGetProperty(device, key);
    int value = 0;
    if (property && CFGetTypeID(property) == CFNumberGetTypeID()) {
        CFNumberGetValue((CFNumberRef)property, kCFNumberIntType, &value);
    }
    return value;
}

int64_t get_int64_from_TypeRef(CFTypeRef ref) {
    int64_t value = 0;
    if (ref && CFGetTypeID(ref) == CFNumberGetTypeID()) {
        CFNumberGetValue((CFNumberRef)ref, kCFNumberSInt64Type, &value);
    }
    return value;
}

static int keyspec_productId;
static int keyspec_keycode;

void handle_HID_input(void* context, IOReturn result, void* sender, IOHIDValueRef value) {
    printf("handle_HID_input(");
    IOHIDElementRef element = IOHIDValueGetElement(value);
    IOHIDDeviceRef device = IOHIDElementGetDevice(element);

    CFTypeRef ref_vendorID = IOHIDDeviceGetProperty(device, CFSTR(kIOHIDVendorIDKey));
    CFTypeRef ref_productID = IOHIDDeviceGetProperty(device, CFSTR(kIOHIDProductIDKey));
    
    int64_t vendorId = get_int64_from_TypeRef(ref_vendorID);
    int64_t productId = get_int64_from_TypeRef(ref_productID);
    
    int64_t keycode = IOHIDValueGetIntegerValue(value);
    
//    CFNumberGetTypeID();
//    CFNumberGetValue((CFNumberRef)ref_vendorID, kCFNumberSInt64Type, &vendorId);
    
    if (productId!=0 && keycode!=0) {
        //    app.input.pushI(CMD_KEYSPEC, keyspec_productId, keyspec_keycode);
        app.onKeySpec(productId,vendorId, keycode);
        sendkeystroketoapptitled("Untitled");
    }
    
    printf(")handle_HID_input;\n");
}
 

int setup_handle_HID_input() {
    printf("setup_handle_HID_input(");
    IOHIDManagerRef hidManager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
    
    if (!hidManager) {
        fprintf(stderr, "Failed to create IOHIDManager\n");
        return 1;
    }

    // Create a matching dictionary for keyboards
    CFMutableDictionaryRef keyboardMatch = CFDictionaryCreateMutable(kCFAllocatorDefault, 0,
        &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);

    int usagePage = kHIDPage_GenericDesktop;
    int usage = kHIDUsage_GD_Keyboard;

    CFDictionarySetValue(keyboardMatch, CFSTR(kIOHIDDeviceUsagePageKey),
        CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &usagePage));
    CFDictionarySetValue(keyboardMatch, CFSTR(kIOHIDDeviceUsageKey),
        CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &usage));

    CFArrayRef matchArray = CFArrayCreate(kCFAllocatorDefault, (const void**)&keyboardMatch, 1, &kCFTypeArrayCallBacks);
    IOHIDManagerSetDeviceMatchingMultiple(hidManager, matchArray);

    // Register input callback
    IOHIDManagerRegisterInputValueCallback(hidManager, handle_HID_input, NULL);

    // Schedule with current run loop
    IOHIDManagerScheduleWithRunLoop(hidManager, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);

    // Open the manager
    if (IOHIDManagerOpen(hidManager, kIOHIDOptionsTypeNone) != kIOReturnSuccess) {
        fprintf(stderr, "Failed to open IOHIDManager\n");
        return 1;
    }

    printf("Listening for keyboard input via handle_HID_input...\n");
    //CFRunLoopRun(); // Start run loop to receive events
    
    // Cleanup (never reached here normally)
    //CFRelease(hidManager);
    printf(")setup_handle_HID_input;/n");
    return 0;
}


std::string CFStringToStdString(CFStringRef cfStr) {
    if (!cfStr) return "";
    char buffer[1024];
    if (CFStringGetCString(cfStr, buffer, sizeof(buffer), kCFStringEncodingUTF8)) {
        return std::string(buffer);
    }
    return "";
}

pid_t FindAppWithWindowTitle(const std::string& targetTitle) {
    CFArrayRef windowList = CGWindowListCopyWindowInfo(kCGWindowListOptionOnScreenOnly, kCGNullWindowID);
    if (!windowList) return 0;

    CFIndex count = CFArrayGetCount(windowList);
    for (CFIndex i = 0; i < count; ++i) {
        CFDictionaryRef windowDict = (CFDictionaryRef)CFArrayGetValueAtIndex(windowList, i);
        CFStringRef titleRef = (CFStringRef)CFDictionaryGetValue(windowDict, kCGWindowName);
        CFNumberRef pidRef = (CFNumberRef)CFDictionaryGetValue(windowDict, kCGWindowOwnerPID);

        if (!titleRef || !pidRef) continue;

        std::string title = CFStringToStdString(titleRef);
        if (title == targetTitle) {
            pid_t pid = 0;
            CFNumberGetValue(pidRef, kCFNumberIntType, &pid);
            CFRelease(windowList);
            return pid;
        }
    }

    CFRelease(windowList);
    return 0;
}

bool ActivateApp(pid_t pid) {
    ProcessSerialNumber psn;
    if (GetProcessForPID(pid, &psn) == noErr) {
        return (SetFrontProcess(&psn) == noErr);
    }
    return false;
}

bool SendKeystroke(CGKeyCode keycode) {
    CGEventRef keyDown = CGEventCreateKeyboardEvent(NULL, keycode, true);
    CGEventRef keyUp = CGEventCreateKeyboardEvent(NULL, keycode, false);

    if (!keyDown || !keyUp) return false;

    CGEventPost(kCGHIDEventTap, keyDown);
    CGEventPost(kCGHIDEventTap, keyUp);

    CFRelease(keyDown);
    CFRelease(keyUp);
    return true;
}


// continue from
// https://chatgpt.com/c/67ff99c1-7c50-8012-8cfa-753c309f32b6
int sendkeystroketoapptitled(std::string targetWindowTitle) {
    std::string targetTitle = "Untitled - TextEdit"; // change this
    CGKeyCode keycode = 0; // 'A' key on macOS

    pid_t targetPid = FindAppWithWindowTitle(targetTitle);
    if (targetPid == 0) {
        std::cerr << "Window not found.\n";
        return 1;
    }

    std::cout << "Found target window. PID: " << targetPid << "\n";

    if (!ActivateApp(targetPid)) {
        std::cerr << "Could not activate target app.\n";
        return 1;
    }

    usleep(200000); // Wait a bit for app to focus

    if (!SendKeystroke(keycode)) {
        std::cerr << "Failed to send keystroke.\n";
        return 1;
    }

    std::cout << "Sent keystroke.\n";
    return 0;
}



char nativeKeyCodeToAsciiChar(int keycode) {
    TISInputSourceRef currentKeyboard = TISCopyCurrentKeyboardInputSource();
    if (!currentKeyboard) return 0;

    CFDataRef layoutData = (CFDataRef)TISGetInputSourceProperty(currentKeyboard, kTISPropertyUnicodeKeyLayoutData);
    if (!layoutData) {
        CFRelease(currentKeyboard);
        return 0;
    }

    const UCKeyboardLayout* keyboardLayout = (const UCKeyboardLayout*)CFDataGetBytePtr(layoutData);

    UInt32 deadKeyState = 0;
    UniChar chars[4];
    UniCharCount actualStringLength = 0;

    OSStatus status = UCKeyTranslate(keyboardLayout,
                                     (UInt16)keycode,
                                     kUCKeyActionDisplay,
                                     0, // no modifiers
                                     LMGetKbdType(),
                                     0,
                                     &deadKeyState,
                                     sizeof(chars) / sizeof(chars[0]),
                                     &actualStringLength,
                                     chars);

    CFRelease(currentKeyboard);

    if (status == noErr && actualStringLength > 0 && chars[0] < 128) {
        return (char)chars[0]; // Return ASCII char
    } else {
        return 0; // Non-printable or error
    }
}


CGEventRef eventTapCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
    
    printf("eventTapCallback(");
    
    if (type == kCGEventKeyDown) {//} || type == kCGEventKeyUp) {
        // Get source device
        CGEventSourceRef source = CGEventCreateSourceFromEvent(event);
        
        if (source) {
            printf("eventTapCallback(");
           // return (app.onEventTap()?event:NULL); // Block the event
            
            int kbdtype = CGEventSourceGetKeyboardType(source); // This is NOT the real vendor ID!
            
            //case Apple Keyboard: 47 (press L)
            //case ho Black One  : 3
            //case Soft          : 47
            
            printf("Got key from keyboard type %d [0x%X]\n", kbdtype, kbdtype);
            CFRelease(source);


            int vendorID = getHIDIntProperty(detectedKeyboards[i].device, CFSTR(kIOHIDVendorIDKey));
            int productID = getHIDIntProperty(detectedKeyboards[i].device, CFSTR(kIOHIDProductIDKey));
            CGKeyCode keyCode = (CGKeyCode)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
                //printf("Keycode: %d\n", keyCode);
            
            bool allow = app.onKeySpec(vendorID, productID,
                                       nativeKeyCodeToAsciiChar(keyCode));
            
            return allow?event:NULL;
            
            // Check if the event is from a detected keyboard
            for (int i = 0; i < keyboardCount; i++) {
                
                int vendorID = getHIDIntProperty(detectedKeyboards[i].device, CFSTR(kIOHIDVendorIDKey));
                 int productID = getHIDIntProperty(detectedKeyboards[i].device, CFSTR(kIOHIDProductIDKey));

                if (vendorID == TARGET_VENDOR_ID && productID == TARGET_PRODUCT_ID)  {
                    
                  //  detectedKeyboards[i].device->IOHIDDeviceGetProperty(kIOHIDVendorIDKey);
                   
                    
                    printf("Blocked key from keyboard Vendor 0x%X, Product 0x%X\n",
                           detectedKeyboards[i].vendorID, detectedKeyboards[i].productID);
                    return NULL;// Block the event
                }
                
                
                
            }
        }
    }
    
    printf(")eventTapCallback;\n");
    return event; // Allow event to pass normally
}

void CreateKeyboardEventTap() {
    printf("CreateKeyboardEventTap(");
    // Create an event tap
    eventTap = CGEventTapCreate(              kCGSessionEventTap,
                                kCGHeadInsertEventTap,
                                kCGEventTapOptionDefault,
                                CGEventMaskBit(kCGEventKeyDown)
                                //| CGEventMaskBit(kCGEventKeyUp)
                                , eventTapCallback,
                                NULL
                                );
    
    if (!eventTap) {
        printf("Failed to create CGEventMaskBit(kCGEventKeyDown) | CGEventMaskBit(kCGEventKeyUp) event tap.\n");
    } else {
        CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
        CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
        CGEventTapEnable(eventTap, true);
        printf("CGEventMaskBit(kCGEventKeyDown) event tap Enabled.\n");
        
        // To doEvents for this callback this is how we Process events for the event tap
        // CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0.001, false);  // small timeout
        //
        
    }
    printf(")CreateKeyboardEventTap;\n");
}

int main( int argc, char** argv )
{
  

    test_systemevent_h_integration("TextEdit", "z");
    
#if defined(MACOSX) || defined(IOS)
    MetalStuff();
#endif
    
    Try1();
    ImGui::CreateContext();
  glutInit( &argc, argv );
  //app.coord.setPortrait();

  auto mainDisplayId = CGMainDisplayID();
  int factor = 1;

  int kWindowWidth = (int)CGDisplayPixelsWide(int(mainDisplayId)*factor);//app.getBackgroundSize().x;
  int kWindowHeight = (int)CGDisplayPixelsHigh(int(mainDisplayId)*factor);//app.getBackgroundSize().y;

  app.coord.setWorkstationSize(i2(kWindowWidth, kWindowHeight));
    
  glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
    
    i2 worksationsize = i2(abs(kWindowWidth), abs(kWindowHeight));
        i2 a51(2400, 1080);
        
    i2 gameSizeRequirement = app.getBackgroundSize();//.half().mult(0.8);//half to let it fit in mac window

        if (app.getOrientation() == app.coord.ORIENTATION_LANDSCAPE)
            gameSizeRequirement = gameSizeRequirement.flip();


        i2 winemusize = gameSizeRequirement;
        if (winemusize.x == 0 && winemusize.y == 0) {
            if (app.getOrientation() == app.coord.ORIENTATION_LANDSCAPE) {
                winemusize.x = worksationsize.x;
                winemusize.y = worksationsize.y - 75;
            }
            else {
                winemusize.x = worksationsize.x / 3.5;
                winemusize.y = worksationsize.y - 75;
            }
        }
    
    if (app.AlgeApp::hideOnStartup()) {
        winemusize.x =1;
        winemusize.y =1;
    }
    
    app.coord.setResolutionReported(winemusize,1.0);//macos is non retina
    app.topSide = 0;
    app.bottomSide = winemusize.y;
    app.leftSide = 0;
    app.rightSide = winemusize.x;
    app.screenScale = float(winemusize.x) / float(kWindowWidth);
    //app.originX = winemusize.x/2;
    //app.originY = winemusize.y/2;

    
    glutInitWindowSize( winemusize.x, winemusize.y );
  
  glutReshapeWindow(winemusize.x, winemusize.y);
  glutInitWindowSize( 0, 0 );
  int theWindowHandle = glutCreateWindow( ResPath );
  glutSetWindow( theWindowHandle );
    appSize(winemusize.x,winemusize.y,1);


  glutDisplayFunc( HandleDisplay );
  
  glutIdleFunc( HandleIdle );
  glutKeyboardFunc( HandleKeyBoard );
  glutKeyboardUpFunc(HandleKeyBoardUp);
  glutSpecialFunc( HandleSpecialKey );
  glutMouseFunc( HandleMouse );
  glutMotionFunc( HandleMotion );
  
  glutTimerFunc(0, reset_window_title, 50);
    
 // HandleMotion( 0, 0 );
    
  glClearColor( 0.3f, 0.3f, 0.3f, 1.0 );
  glEnable( GL_DEPTH_TEST );
    
 // FILE* f = fopen("CANDIDATE.h", "w");
 //  fputs(FULLYQUALIFIEDPATH, f);
//  fclose(f);
 //   ImGui_ImplAlgeSDK_Main(winemusize.x,winemusize.y,1);
    
    
    ImGui_ImplAlgeSDK_Main(app.coord.getResolutionForGuiUsage().x,app.coord.getResolutionForGuiUsage().y, app.coord.getGuiScale());
    glutPassiveMotionFunc(ImGui_ImplGLUT_MotionFunc);
    
    
    
#ifdef __FREEGLUT_EXT_H__
    glutMouseWheelFunc(ImGui_ImplGLUT_MouseWheelFunc);
#endif
    glutKeyboardUpFunc(ImGui_ImplGLUT_KeyboardUpFunc);
    glutSpecialUpFunc(ImGui_ImplGLUT_SpecialUpFunc);
    
    FindAppName();  sprintf(app.rm.resourcepath, "%s", argv[0]);
#ifndef NO_NATS
    netmsg.SubscribeConnect("e.*.out", false);// "Evolution");
    callBackIn = &onRemoteCommand; //Some Error Recheck Callback scheme
#endif
  char* l = strrchr(app.rm.resourcepath, '.');
	
  if (l) l[0] = 0;
   l = strrchr(app.rm.resourcepath, '/');
  if (l) l[0] = 0;
	
  strcat(app.rm.resourcepath, "/");
  strcat(app.rm.resourcepath, ALGEAPPNAME);
  strcat(app.rm.resourcepath, ".app/Contents/Resources/Data");
  
 // initAL(app.rm.resourcepath);
#ifndef NO_FMOD
    FMOD_Init();
#else
    initAL(app.rm.resourcepath);
#endif
   app.rm.Init(app.rm.resourcepath);
 //   app.resolutionReported.x = kWindowWidth;
 //   app.resolutionReported.y = kWindowHeight;
    
    strcpy(ResPath, app.rm.resourcepath);
    strcat(ResPath, "/");
    app.Init(app.rm.resourcepath);

    if (argc>0) {
        for (int i=0; i<argc; i++) {
            strcat(commandline, argv[i]);
            strcat(commandline, " ");
        }
        app.input.pushP(CMD_COMMANDLINE, (void*) commandline, 0);
    }
  startTime = glutGet(GLUT_ELAPSED_TIME);
  deltaT = 0;
  time1.start();
  lastTime = time1.getElapsedTimeInSec();
  
#ifndef NO_FONTLIB
  font = nullptr;
   
  // https://github.com/jtsiomb/libdrawtext/blob/master/examples/simple/simple.c
    
  if(!(font = dtx_open_font("/Library/Fonts/AppleGothic.ttf", 24))) {
        fprintf(stderr, "failed to open font\n");
   }

  // XXX select the font and size to render with by calling dtx_use_font
//if you want to use a different font size, you must first call:
   // dtx_prepare(font, size) once.
   //
   
  dtx_use_font(font, 24);
    

    
    
#endif
    
    detectKeyboards();
      
    for (size_t i = 0; i < keyboards.size(); ++i) {
          int vendorID = keyboards[i].first;
          int productID = keyboards[i].second;
//printf("MAIN:: Detected Keyboard #%zu - Vendor ID: 0x%X, Product ID: 0x%X\n", i+1,vendorID, productID);
          app.input.pushI(DETECTED_KEYBOARD, vendorID, productID);
        
    }
    
    

    
  glutReshapeFunc( HandleReshape );
  glutMainLoop( );
  
  ImGui_ImplAlgeSDK_Shutdown();
  return 0;
}

#include "../../SDKSRC/Base/externit.cpp"

