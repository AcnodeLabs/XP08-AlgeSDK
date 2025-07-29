#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <assert.h>
#include <string.h>

using namespace std;

#include "importgl.h"
#include "app.h"

#ifndef NO_IMGUI
// XGui stuff


#include "../../../imgui/imgui.h"

float m_fontScale = 0.5;

#ifdef ANDROID
#include <string>

int ANativeWindow_getWidth(void* dummy) { return 100; }
int ANativeWindow_getHeight(void* dummy) { return 200; }
#include <android/input.h>
#include <android/keycodes.h>
#include <android/log.h>
#include "../../Platforms/Alge-Android-Jetpack/imgui_impl_android.h"
#include "../../Platforms/Alge-Android-Jetpack/imgui_impl_opengl2.h"
#endif

#ifdef MACOS
#include "../../Platforms/Alge-iOS-Arm/imgui_impl_opengl2.h"
#endif

#ifdef IOS
#define IMGUI_DEFINE_MATH_OPERATORS
#include <string>
#include "../../Platforms/Alge-iOS-Arm/imgui_impl_opengl2.h"
#endif

#ifdef WINDOWS
#include "../../Platforms/Alge-Windows/imgui_impl_opengl2.h"
#endif

#ifndef WINDOWS
class GuiFont : public ImFont {
    ImFont* thiz;
public:
    GuiFont() {}

    void Use(std::string fontfile, float size) {
        thiz = ImGui::GetIO().Fonts->AddFontFromFileTTF(fontfile.c_str(), size * m_fontScale);
    }

    void Use(std::string font) {

    }

};
GuiFont imguiFont;
#endif






static bool show_demo_window = true;
static bool show_another_window = false;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

#ifndef NO_FMOD
#include "../../SDKSRC/Base/fmod/framework.hpp"
#endif
//static bool show_demo_window = true;
//static bool show_another_window = false;
//static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);



void ImGui_ImplAlgeSDK_Main(int x, int y, int guiScale) {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.DisplaySize.y = y * guiScale;
    io.DisplaySize.x = x * guiScale;

#ifdef ANDROID
    ANativeWindow* an;// = ANativeWindow_fromSurface();
    ImGui_ImplAndroid_Init(an);
#endif

    m_fontScale = guiScale;
    ImGui::StyleColorsClassic();

}

void ImGui_ImplAlgeSDK_AfterRender(char* msg)
{
    ImGui::Render();
    //ImGui_ImplOpenGL2es_RenderDrawData(ImGui::GetDrawData(),1);
    //ImGui_ImplOpenGL2es_RenderDrawData(ImGui::GetDrawData());
   // ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    //#ifndef ANDROID
    //,msg
    //#endif
    //);
}

void ImGui_ImplAlgeSDK_BeforeRender()
{
	ImGui_ImplOpenGL2_NewFrame();
#ifdef ANDROID
    ImGui_ImplAndroid_NewFrame();
#endif
	ImGui::NewFrame();
    ImGui::GetIO().FontGlobalScale = m_fontScale;
}

void ImGui_ImplAlgeSDK_Shutdown() {
    ImGui_ImplOpenGL2_Shutdown();
#ifdef ANDROID
    ImGui_ImplAndroid_Shutdown();
#endif
    ImGui::DestroyContext();
}


void GuiStarts() {
  //  ImGui_ImplAlgeSDK_BeforeRender();
    ImGui_ImplOpenGL2_NewFrame();

}

void GuiEnds() {
    static char msg[1024] = {0,0};
    ImGui_ImplAlgeSDK_AfterRender(msg);
}

////////////////////////

#endif // of NO_IMGUI check

//#include CANDIDATE
#ifdef ANDROID
#define CBASE "CBaseV1_2.h"
#include CBASE
#include "../../Platforms/candidate.h"
#else
#define CBASE "../../SDKSRC/Base/CBaseV1_2.h"
#include CBASE
#include "CANDIDATE.h"
#endif

#define RUN_LENGTH  (20 * CAMTRACK_LEN)
#undef PI
#define PI 3.1415926535897932f
#define RANDOM_UINT_MAX 65535

App game;

/* Capped conversion from float to fixed.
static long floatToFixed(float value)
{
    if (value < -32768) value = -32768;
    if (value > 32767) value = 32767;
    return (long)(value * 65536);
}

#define FIXED(value) floatToFixed(value)
*/

char ResPath[MAX_PATH];
XHttpSocket sck;
XHttpSocket msck;
bool init_complete = false;

const int TITLEBARHEIGHT = 0;
int XERR = 0;
int YERR = 0;

i2 param2coords(int px, int py) {
    int strange_factor = 1;

#ifdef ANDROID
    XERR = 0;
    YERR = 95;
#endif

    int xPos = (px) * strange_factor  - XERR;
    int yPos = (py) * strange_factor -TITLEBARHEIGHT -YERR;
    return i2(xPos, yPos);
}

static int p1, p2;
const int    left_butt = 0;
const int    right_butt = 1;
const int    middle_butt = 2;

static bool clickonce = true;

//Ensure Processing of TouchEnd Event as operating thu PEGs is intermittent
void onTouchEnd()
{
    ImGuiIO& io = ImGui::GetIO();

    io.AddMouseSourceEvent(ImGuiMouseSource_TouchScreen);

    //if (io.WantCaptureMouse)
    //{
    //        io.AddMousePosEvent(p1, p2-TITLEBARHEIGHT);
    io.AddMouseButtonEvent(left_butt, false);
    io.AddMouseButtonEvent(right_butt, false);
    io.AddMouseButtonEvent(middle_butt, false);
    clickonce = true;
}

void appPushI(int command, int pp1, int pp2) {
    p1 = pp1;
    p2 = pp2;
   if (command==14739 || command == 4369) return;// TODO

    if (command == CMD_PAUSE) {
        if (game.onPause()) return;
    }

    if (command == CMD_UNPAUSE) {
        if (game.onResume()) return;
    }

/*
#ifndef NO_IMGUI
    ImGuiIO& io = ImGui::GetIO();
    if (command == CMD_TOUCH_START) {
        i2 p = param2coords(p1,p2);

        io.MouseDown[0] = true;
        io.MousePos = ImVec2((float)p.x, (float)p.y);

        io.AddMouseButtonEvent(0, true);
    }
    if (command == CMD_TOUCHMOVE) {
        i2 p = param2coords(p1,p2);
        io.MouseDown[0] = true;
        io.MousePos = ImVec2((float)p.x, (float)p.y);
        io.MouseDown[0] = true;
        io.AddMousePosEvent(p.x,p.y);
    }
    
    if (command == CMD_TOUCH_END) {
        i2 p = param2coords(p1,p2);
        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = ImVec2((float)p.x, (float)p.y);
        io.MouseDown[0] = false;
        io.AddMouseButtonEvent(0, false);
    }
#endif
*/



    const int TITLEBARHEIGHT = game.getPreferredTitleCorrection();
#ifndef NO_IMGUI
    ImGuiIO& io = ImGui::GetIO();
    io.MousePos = ImVec2((float)p1, (float)p2-TITLEBARHEIGHT);
#endif
    if (command == CMD_TOUCH_START) {
#ifndef NO_IMGUI
        io.AddMouseSourceEvent(ImGuiMouseSource_TouchScreen);
        if (io.WantCaptureMouse)
        {
            io.AddMousePosEvent(p1, p2-TITLEBARHEIGHT);
            if (clickonce) {
                io.AddMouseButtonEvent(left_butt, true);
                clickonce = false;
            }  else
                command = CMD_TOUCH_END;//android is not triggering this

        } else
                game.input.pushI(CMD_TOUCH_START, p1, p2);
#else
        game.input.pushI(CMD_TOUCH_START, p1, p2);
#endif
      //  touching = true;
    }


    if (command == CMD_TOUCHMOVE_MULTI2) {

#ifndef NO_IMGUI
        io.AddMouseSourceEvent(ImGuiMouseSource_TouchScreen);
  //      io.AddMouseWheelEvent(p1, p2-TITLEBARHEIGHT);// need to study
        game.input.pushI(CMD_TOUCHMOVE_MULTI2, p1, p2);
#else
        game.input.pushI(CMD_TOUCHMOVE_MULTI2, p1, p2);
#endif
        //touching = true;

    }

    if (command == CMD_TOUCHMOVE) {

#ifndef NO_IMGUI
        io.AddMouseSourceEvent(ImGuiMouseSource_TouchScreen);
        io.AddMousePosEvent(p1, p2-TITLEBARHEIGHT);
        game.input.pushI(CMD_TOUCHMOVE, p1, p2);
#else
        game.input.pushI(CMD_TOUCHMOVE, p1, p2);
#endif
        //touching = true;

    }

    if (command == CMD_TOUCH_END) {

#ifndef NO_IMGUI
        io.AddMouseSourceEvent(ImGuiMouseSource_TouchScreen);

        //if (io.WantCaptureMouse)
        //{
    //        io.AddMousePosEvent(p1, p2-TITLEBARHEIGHT);
            io.AddMouseButtonEvent(left_butt, false);
            io.AddMouseButtonEvent(right_butt, false);
            io.AddMouseButtonEvent(middle_butt, false);

        //} else
            game.input.pushI(CMD_TOUCH_END, p1, p2);
#else
        game.input.pushI(CMD_TOUCH_START, p1, p2);
#endif
     //   touching = true;
        clickonce = true;
    }

}
void appInit(char *sz) {
#ifndef NO_FMOD
    FMOD_Init();
#endif
	strcpy(ResPath, sz);

    i2 iphone6(1334, 750);

#ifdef ANDROID
    i2 dev = size_note10;

#endif
#ifdef IOS
    i2 dev = iphone6;
#endif

   float gs =1;
#ifdef IOS
    gs = 0.5;//Assume Retina on iOS devoce
#endif
    
    bool ORIENT = game.getOrientation();
    game.coord.setOrientation(ORIENT);
    i2 coords_portrait(dev.y,dev.x);
    i2 coords_landscape(dev.x,dev.y);
    game.coord.setResolutionReported(ORIENT==Coord::ORIENTATION_PORTRAIT?coords_portrait:coords_landscape, gs);

    i2 ge = game.coord.getResolutionForGuiUsage();
    i2 gu = game.coord.getResolutionForGuiUsage();

    appSize(ge.x,ge.y,gs);
#ifndef NO_IMGUI
    ImGui_ImplAlgeSDK_Main(gu.x,gu.y,game.coord.getGuiScale());
    ImGui_ImplOpenGL2_Init();
#endif
 //   game.rm.Init(sz);
    game.Init0(sz, 'A');
   // game.Init(sz);

    //game.getCurrentDay();

    init_complete = true;
#if defined(__ANDROID__)
    ImGui_ImplAndroid_InitX(gu.x,gu.y);
    ImGui::GetStyle().TouchExtraPadding = ImVec2(4.0F, 4.0F);
#endif
}

void appSize(int w, int h, int scale) {

    game.screenScale = scale;
    game.coord.setResolutionReported(i2(w,h),1);
    game.input.pushI(CMD_SCREENSIZE, game.coord.getResolutionForGameEngine().x , game.coord.getResolutionForGameEngine().y);
    game.CamReshape(game.coord.getResolutionForGameEngine());
    game.aCamera.custom_type = 0xCA;
    //if (game.resolutionReported.x<game.resolutionReported.y) gamelandscape= false;
    //game.CamReshape(i2(game.resolutionReported.x,game.resolutionReported.y)); it will be called by game Init
}
void appDeinit(){
#ifndef NO_IMGUI
    ImGui_ImplAlgeSDK_Shutdown();
#endif
#ifndef NO_FMOD
    FMOD_Deinit();
#endif
}

void* lastP1 = 0;
void* lastP2 = 0;
int lastI1 = 0;
int lastI2 = 0;

char filename[256];


short notified = 0;
char errsz[64];

int appPull() {
	PEG::CMD* a = game.output.pull();
	lastP1 = a->param1;
	lastP2 = a->param2;

	int ae = glGetError();
	if (ae) {
		if (!notified) {
			
			sprintf(errsz, "glError=%d", ae);
			//game.output.pushP(CMD_MSG, $ errsz, 0);
			notified = 1;
		}
	}
	
	if (a->command>=CMD_TEXSET0 && a->command<=CMD_TEXSET3) {
		
	//	game.rm.LoadTGA((TextureImage* )lastP2 , (char*)lastP1);
		
	}

	if (a->command==CMD_PAUSE) {
	    game.paused = true;
	}

    if (a->command==CMD_UNPAUSE) {
        game.paused = false;
    }

    switch (a->command) {
       case CMD_SNDSET0:
       case CMD_SNDSET1:
       case CMD_SNDSET2:
       case CMD_SNDSET3:
            {
#ifndef NO_FMOD
              // sndSet((char*)c->param1, c->command-CMD_SNDSET, c->i2);
                string fullpath = string(game.rm.resourcepath) + string("/")+ string((char*)a->param1);
                FMOD_Set((a->command - CMD_SNDSET), (char*)fullpath.c_str());
#endif
            }
       break;
      
         case CMD_SNDPLAY0:
         case CMD_SNDPLAY1:
         case CMD_SNDPLAY2:
         case CMD_SNDPLAY3:
                {
                  //  sndPlay(a->command-CMD_SNDPLAY);
#ifndef NO_FMOD
                    FMOD_Play(a->command - CMD_SNDPLAY);
#endif
                }
    }
    
	lastI1 = a->i1;
	lastI2 = a->i2;
	
	return a->command;
}

void* appP1() { return (void*)lastP1;}
void* appP2() { return (void*)lastP2;}

char* appSzlastP1() {
	return (char*)lastP1;
}

int appIntlastP1() {
	return lastI1;
}

int appIntlastP2() {
	return lastI2;
}


void appAccelerometerSz(char* accelValues) {

}

static void prepareFrame(int width, int height)
{
    if (init_complete==false) return;
    glViewport(0, 0, width, height);
	
    glClearColorx((GLfixed)(0.0f * 65536),
                  (GLfixed)(0.0f * 65536),
                  (GLfixed)(0.0f * 65536), 0x10000);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    aluPerspective(45, (float)width / (float)height, 0.05f, 150);
	
    glMatrixMode(GL_MODELVIEW);
	
    glLoadIdentity();
}


char msg[256];
void appRender(float tick, int width, int height, int accelX, int accelY, int accelZ)
{
    if (init_complete==false) return;
#ifndef NO_IMGUI
    ImGui_ImplAlgeSDK_BeforeRender();
#endif
	prepareFrame(width,height);
	if (accelX==0 && accelY==0 && accelZ==0) accelY = -9.8*100;
#ifdef ANDROID
	if (!game.paused) {
	    game.Render(tick / 1000000., accelX, accelY, accelZ);
    }
#else
    game.Render(tick/1.0, accelX , accelY , accelZ );
#endif
#ifndef NO_IMGUI
    ImGui_ImplAlgeSDK_AfterRender((char*)msg);
#endif
  //  game.input.pushP(CMD_YOUTUBE_SHOW,msg,msg);
    
	return;
}

void appRenderJ(int tickE6, int width, int height, int accelX, int accelY, int accelZ)
{
    float tick = (tickE6 / 1000000.0f);
	appRender( tick, width, height, accelX, accelY, accelZ);
}

//defis imported from alge-gnat-mac
//TODO This code is common in Platforms iOS/device-app.cpp and OSX/alge-mac.cpp
// This part ahhead can be included from a single file placed in SDKSRC/Ada folder

extern "C++" void alAlphaTest(int set_unset, float fA) {
    
    if (set_unset) {
        glAlphaFunc(GL_GREATER, fA);
        glEnable(GL_ALPHA_TEST);
        glEnable(GL_DEPTH_TEST);
      //   glDisable(GL_DEPTH_TEST);
        //  glEnable(GL_BLEND);
        //  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        //  glBlendEquation(GL_FUNC_ADD);
    } else {
       glDisable(GL_ALPHA_TEST);
    }
}

extern "C++" void alLoadIdentity() {
    glLoadIdentity();
}

extern "C++" void alPushMatrix() {
    glPushMatrix();
}

extern "C++" void alPopMatrix() {
    glPopMatrix();
}

extern "C++" void alLoadModel(char* alx, char* tga, int id, float size) {
	string fname = string(alx);
	if (fname.find(".alx") != string::npos)
        game.rm->loadAlxModel((char*)alx, (char*)tga, id, size);
	else //generic 
        game.rm->loadGenericAsset(fname, id);
}

int model_counter = -1;

int alLoadModel(ResourceInf* resInf) {
	alLoadModel((char*)resInf->alx.c_str(), (char*)resInf->tex.c_str(), ++model_counter, resInf->scale);
	return model_counter;
}

int alReserveModelId() {
	++model_counter;
	return model_counter;
}

extern "C++" void alDrawModelTranslateRotate(int id, float posx , float posy, float posz,
                                           float angle,float x, float y,float z,
                                           int rotatefirst ,
                                           int billboard )
{
    glPushMatrix();
    if (rotatefirst!=0)  {
        glRotatef(angle, x,y,z);
        glTranslatef(posx,posy,posz);
    } else {
        glTranslatef(posx,posy,posz);
        glRotatef(angle, x,y,z);
    }
    
    if (game.rm->models[id]->loaded) {
        if (billboard==1) alBillboardBegin();
        game.rm->models[id]->glDraw();
        if (billboard==1) alBillboardEnd();
    }
    glPopMatrix();
}

extern "C++" void alScaleModel(int id, float sx, float sy, float sz) {
    game.rm->models[id]->SetScale(sx,sy,sz);
}

extern "C++" void alDrawModel(int id) {
    game.rm->models[id]->glDraw();
}
extern "C++" void alDrawModel(int id, bool wireframe) {
    game.rm->models[id]->glDraw(wireframe);
}
extern "C++" void alTranslateRotate( float posx , float posy, float posz,
                                  float angle,float x, float y,float z) {
    glTranslatef(posx,posy,posz);
    glRotatef(angle,x,y,z);
}


