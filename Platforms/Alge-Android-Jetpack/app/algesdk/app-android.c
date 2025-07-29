
#include <jni.h>
#include <sys/time.h>
#include <time.h>
#include <android/log.h>
#include <android/native_window_jni.h>
#include <stdint.h>
#include <stdio.h>
#include "app.h"

int   gAppAlive   = 1;

static int  sWindowWidth  = 320;
static int  sWindowHeight = 480;
static int  sDemoStopped  = 0;
static long sTimeOffset   = 0;
static int  sTimeOffsetInit = 0;
static long sTimeStopped  = 0;



/* Call to initialize the graphics state */
void
Java_com_acnodelabs_algeapp_DemoRenderer_nativeInit( JNIEnv*  env , jobject thiz, jstring uri)
{
//    importGLInit();
	// convert Java string to UTF-8
    jobject surface;
    const jbyte *utf8 = (jbyte*)(*env)->GetStringUTFChars(env, uri, NULL);
    appInit(utf8);//, ANativeWindow_fromSurface(env, surface));
    gAppAlive    = 1;
    sDemoStopped = 0;
    sTimeOffsetInit = 0;
	(*env)->ReleaseStringUTFChars(env, uri, (const char*)utf8);
	
}

/* Call to initialize the graphics state */
int
Java_com_acnodelabs_algeapp_DemoGLSurfaceView_nativePull( JNIEnv*  env ,jclass clazz)
{
	int a = appPull();
	return a;
}


/* Call to initialize the graphics state */
void
Java_com_acnodelabs_algeapp_DemoGLSurfaceView_nativeOnTouchEnd( JNIEnv*  env )
{
    onTouchEnd();
    return;
}

/* Call to initialize the graphics state */
jstring
Java_com_acnodelabs_algeapp_DemoGLSurfaceView_nativeSzP1( JNIEnv*  env,jclass clazz)
{
	return (*env)->NewStringUTF(env, (const char *)appSzlastP1());
}

jint
Java_com_acnodelabs_algeapp_DemoGLSurfaceView_nativeIntP1( JNIEnv*  env,jclass clazz)
{
	return appIntlastP1();
}
/* Call to initialize the graphics state */
jint
Java_com_acnodelabs_algeapp_DemoGLSurfaceView_nativeIntP2( JNIEnv*  env,jclass clazz)
{
	return appIntlastP2();
}

/* Call to initialize the graphics state */
void
Java_com_acnodelabs_algeapp_DemoGLSurfaceView_nativePushI( JNIEnv*  env, jobject  thiz, jint cmd, jint x, jint y)
{
	appPushI(cmd,x,y);
}

void
Java_com_acnodelabs_algeapp_DemoRenderer_nativeResize( JNIEnv*  env, jobject  thiz, jint w, jint h )
{
    sWindowWidth  = w;
    sWindowHeight = h;
    __android_log_print(ANDROID_LOG_INFO, "AlgeApp", "resize w=%d h=%d", w, h);
}

/* Call to finalize the graphics state */
void
Java_com_acnodelabs_algeapp_DemoRenderer_nativeDone( JNIEnv*  env ,jclass clazz)
{
    appDeinit();
 //   importGLDeinit();
}

/* This is called to indicate to the render loop that it should
 * stop as soon as possible.
 */
void
Java_com_acnodelabs_algeapp_DemoGLSurfaceView_nativePause( JNIEnv*  env ,jclass clazz)
{
    sDemoStopped = !sDemoStopped;
    if (sDemoStopped) {
        /* we paused the animation, so store the current
         * time in sTimeStopped for future nativeRender calls */
 //       sTimeStopped = _getTime();
    } else {
        /* we resumed the animation, so adjust the time offset
         * to take care of the pause interval. */
  //      sTimeOffset -= _getTime() - sTimeStopped;
    }
}

static float _getElapsedTime() {
    static float OldTime = 0;
    struct timeval  now;
    gettimeofday(&now, NULL);
    int seconds = ((int)now.tv_sec);
    int microseconds = ((int)now.tv_usec);
    float time = microseconds / 1000.0;
    float elapsed = time - OldTime;
    if (elapsed<=0) elapsed = 0.0166666667;
    OldTime = time;
    return elapsed;
}

/* Call to render the next GL frame */
void
Java_com_acnodelabs_algeapp_DemoRenderer_nativeRender( JNIEnv*  env, jobject  thiz, jint ax100, jint ay100, jint az100)
{
	float accelX = ax100 / 100.0f;
	float accelY = ay100 / 100.0f;
	float accelZ = az100 / 100.0f;
	
   float ielapsed = (float)_getElapsedTime() * 1E6 / 1E3;
   // if (sDemoStopped) elapsed = 0;
	appRender( ielapsed, sWindowWidth, sWindowHeight, ax100,ay100,az100);

}
