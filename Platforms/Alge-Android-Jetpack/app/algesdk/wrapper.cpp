#include "../../../Alge-Android/app/src/main/cpp/device-app.cpp"
#include <jni.h>
#include <sys/time.h>
#include <time.h>
#include <android/log.h>
#include <stdint.h>
#include <stdio.h>


int   gAppAlive   = 1;

static int  sWindowWidth  = 320;
static int  sWindowHeight = 480;
static int  sDemoStopped  = 0;
static long sTimeOffset   = 0;
static int  sTimeOffsetInit = 0;
static long sTimeStopped  = 0;

App app;

jstring jString(JNIEnv *env, string a) { return env->NewStringUTF(a.c_str()); }
string String(JNIEnv *env, jstring s) {
    jboolean isCopy;
    const char *convertedValue = (env)->GetStringUTFChars(s, &isCopy);
    std::string str = convertedValue;
    return str;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_acnodelabs_algeapp_MainActivity_appInit(JNIEnv *env, jobject thiz, jstring path) {
    // TODO: implement appInit()
    string sp = String(env, path);
    appInit((char*)sp.c_str());

    gAppAlive    = 1;
    sDemoStopped = 0;
    sTimeOffsetInit = 0;

}
extern "C"
JNIEXPORT void JNICALL
Java_com_acnodelabs_algeapp_MainActivity_appDeinit(JNIEnv *env, jobject thiz) {
    appDeinit();
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

extern "C"
JNIEXPORT void JNICALL
Java_com_acnodelabs_algeapp_MainActivity_appRender(JNIEnv *env, jobject thiz, jfloat tick,
                                                   jint width, jint height, jint accel_x,
                                                   jint accel_y, jint accel_z) {
    float accelX = accel_x / 100.0f;
    float accelY = accel_y / 100.0f;
    float accelZ = accel_z / 100.0f;

    int ielapsed = _getElapsedTime() * 1E6 / 1E3;
    // if (sDemoStopped) elapsed = 0;
    appRenderJ( ielapsed, sWindowWidth, sWindowHeight, accel_x,accel_y,accel_z);
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_acnodelabs_algeapp_MainActivity_appPull(JNIEnv *env, jobject thiz) {
    int a = appPull();
    return a;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_acnodelabs_algeapp_MainActivity_appPushI(JNIEnv *env, jobject thiz, jint cmd, jint x,
                                                  jint y) {
    // TODO: implement appPushI()
    appPushI(cmd,x,y);
}



