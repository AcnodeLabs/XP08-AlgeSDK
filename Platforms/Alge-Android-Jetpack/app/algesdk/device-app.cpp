#ifndef ANDROID
#define ANDROID
#endif

#ifdef ANDROID
#define NO_FONTLIB
#define NO_FMOD
#endif

#include "../../../../SDKSRC/Base/deviceapp.cpp"
class App;