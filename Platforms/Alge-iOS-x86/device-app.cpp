#define XCODE_BUILD
#define IOS

#ifdef IOS
#define NO_NATS
#define ES_ONLY
#define NO_FONTLIB
//#define NO_IMGUI
#define NO_FMOD
#define GLES_SILENCE_DEPRECATION
#endif


#ifdef IOS
//#include "xgui.hpp"
#endif

#ifdef ANDROID
//#include "xgui.hpp"
#endif


#include "../../SDKSRC/Base/deviceapp.cpp"
