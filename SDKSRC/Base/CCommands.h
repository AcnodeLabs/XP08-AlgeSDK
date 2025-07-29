#define CMD_LOG 0x3001
#define CMD_MSG 0x3002
#define CMD_NATSMSG 0x3121

#define CMD_CUSTOM 0x4400
#define CMD_VOID 0x3003
#define CMD_ERR 0x300
#define CMD_SNDSET 0x3040
#define CMD_SNDSET0 0x3040
#define CMD_SNDSET1 0x3041
#define CMD_SNDSET2 0x3042
#define CMD_SNDSET3 0x3043
#define CMD_SNDSET4 0x3044
#define CMD_SNDSET5 0x3045
#define CMD_SNDSET6 0x3046

#define CMD_SNDPLAY 0x3050
#define CMD_SNDPLAY0 0x3050
#define CMD_SNDPLAY1 0x3051
#define CMD_SNDPLAY2 0x3052
#define CMD_SNDPLAY3 0x3053
#define CMD_SNDPLAY4 0x3054
#define CMD_SNDPLAY5 0x3055
#define CMD_SNDPLAY6 0x3056

#define CMD_TEXSET0 0x3230
#define CMD_TEXSET1 0x3231
#define CMD_TEXSET2 0x3232
#define CMD_TEXSET3 0x3233
#define CMD_TEXID 0x3205
#define CMD_SNDLOOP 0x3055
#define CMD_VIDSET 0x3007
#define CMD_VIDPLAY 0x3008
#define CMD_DONE 0x3101
#define CMD_PAUSE 0x3333
#define CMD_UNPAUSE 0x3334
#define CMD_BEGIN 0x1111
#define CMD_END 0xffff

#define CMD_TOUCH_START 0x3300

#define CMD_TOUCH_STARTR 0x3301

#define CMD_TOUCH_END 0x3303
#define CMD_TOUCHMOVE 0x3321
#define CMD_TOUCHMOVER 0x3322
#define CMD_TOUCHMOVE_MULTI2 0x3309

#define CMD_MOUSEMOVE 0x3331
#define CMD_MOUSEWHEEL 0x3334

#define CMD_TOUCH_START2 0x3400
#define CMD_TOUCH_END2 0x3401
#define CMD_TOUCHMOVE2 0x3421

#define CMD_PINCH_START 0x3500
#define CMD_PINCH_END 0x3501
#define CMD_PINCH_MOVE_OUT 0x3521
#define CMD_PINCH_MOVE_IN 0x3522
#define CMD_LONGPRESS 0x3529

#define CMD_SCREENSIZE 0x3993
#define CMD_SYSTEMTIME 0x3373
#define CMD_SYSTEMDATE 0x3374
#define CMD_TITLE 0x3011
#define CMD_SPAWN 0x3012
#define CMD_HHMM 0x3013
#define CMD_MMDD 0x3014
#define CMD_OPEN 0x3015
#define CMD_BROWSE 0x3016

#define CMD_YOUTUBE_SHOW 0x3875
#define CMD_YOUTUBE_CLOSE 0x3877
#define CMD_YOUTUBE_READY 0x3876
#define CMD_ALLSET 0x3993
#define CMD_KEYDOWN 0x7998
#define CMD_KEYSPEC 0x8998  //Provides vendorID and productID to identify keyboard

#define CMD_KEYUP 0x7999
#define CMD_TOAST 0x3022
#define CMD_COMMANDLINE 0x8765
#define CMD_REMOTE_COMMAND 0x8767
#define CMD_SERVICE_RESULT 0x8727
#define CMD_MOUSE_REPOSITION 0x8887
#define CMD_USEGAMEPAD 0x8876
#define CMD_GAMEPAD_EVENT 0x8676
#define CMD_SETTINGS_SCREEN 0x9100

#define DETECTED_KEYBOARD 0x6010
/// Use this to request platform to setup keyboard Tap to Allow/Disallow Keys
/// as per App requirement
#define KEYEVENT_TAP 0x6060
#define HID_INPUT    0x6061
#define CMD_SENDKEYTO_PID  0x6063
#define CMD_SENDKEYTO_APP  0x6064

#ifdef WIN32
#define KEY_FWD 38
#define KEY_BAC 40
#define KEY_LFT 37
#define KEY_RGT 39
#define KEY_PGUP 41
#define KEY_PGDN 42
#define KEY_0 48
#define KEY_9 57
#define KEY_PLUS 43

#else
#define KEY_FWD 101
#define KEY_BAC 103
#define KEY_LFT 100
#define KEY_RGT 102
#define KEY_PGUP 104
#define KEY_PGDN 105
#define KEY_0 48
#define KEY_9 57
#define KEY_PLUS 43

#endif // WIN32

//Deprecated Use KEY_XX defs instead
// 
#define AL_KEY_DOT (190)
#define AL_KEY_PLUS (58)
#define AL_KEY_MINUS (60)
//#define AL_KEY_LEFT (-12)
//#define AL_KEY_UP (-11)
//#define AL_KEY_RIGHT (-10)
//#define AL_KEY_DOWN (-9)
#define AL_KEY_ALPHA (16)
#define AL_KEY_SPACE (-17)
#define ALKEY(k) (AL_KEY_ALPHA + (k-'A'))



#define $ (void*)(char*)
/*
int cmdCode(string name) {
	return 0;
}

string cmdName(char* r) {
	return "";
}

void convertToPEG(PEG::CMD* p, char* r) {
	string parseable(r);
	//p->command = cmdCode(cmdName(r));
}
*/
