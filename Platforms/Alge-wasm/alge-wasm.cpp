//
//  alge-mac.cpp
//  Alge-Mac
//
//  Created by Bilal on 6/20/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#define WASM

#ifdef __EMSCRIPTEN__
    #undef GLfloat;
    #include <GL/glut.h>
    #include <GLES2/gl2.h>       // For WebGL 1.0 / OpenGL ES 2.0
    #include <GLES2/gl2ext.h>    // Optional, for extensions

#else
    #ifdef __APPLE__
        #include <GLUT/glut.h>
    #else
        #include <GL/glut.h>
    #endif
#endif
#include <stdio.h>


void glColor3f(GLfloat r, GLfloat g, GLfloat b) {
    glColor4f(r, g, b, 1.0f);
}
////////////////////////////////////////////////////////////////  ES2 WRAPPER CODE // https://chatgpt.com/g/g-6crAbR9j8-opengl-es-graphics-quest/c/687798db-b43c-8012-abac-b1894f23a425
#include <stack>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

static std::stack<glm::mat4> matrixStack;
static glm::mat4 currentMatrix = glm::mat4(1.0f);

// You should pass this matrix to your shader via uniform
extern glm::mat4 modelViewMatrix;

inline void glPushMatrix() {
    matrixStack.push(currentMatrix);
}

inline void glPopMatrix() {
    if (!matrixStack.empty()) {
        currentMatrix = matrixStack.top();
        matrixStack.pop();
    }
}

inline void glLoadMatrixf(const GLfloat* m) {
    currentMatrix = glm::make_mat4(m);
}

inline void glTranslatef(GLfloat x, GLfloat y, GLfloat z) {
    currentMatrix = glm::translate(currentMatrix, glm::vec3(x, y, z));
}

inline void glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z) {
    currentMatrix = glm::rotate(currentMatrix, glm::radians(angle), glm::vec3(x, y, z));
}

inline void glColor4f(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
    // If using shaders, pass to a uniform
    // setColorUniform(glm::vec4(r, g, b, a));
}

inline void glShadeModel(GLenum mode) {
    // Ignore: WebGL does flat/smooth via shaders
}

inline void glAlphaFunc(GLenum func, GLclampf ref) {
    // Emulate in fragment shader with: if (alpha < ref) discard;
}

inline void glDisableClientState(GLenum array) {
    // No-op; VAOs/VBOs used instead
}

inline void gluPerspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar) {
    currentMatrix = glm::perspective(glm::radians(fovy), aspect, zNear, zFar);
}

inline void glOrthof(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar) {
    currentMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
}

// Optional: call this each frame before render
inline void updateShaderMatrix(GLuint program, const char* uniformName) {
    GLint loc = glGetUniformLocation(program, uniformName);
    if (loc >= 0) {
        glUniformMatrix4fv(loc, 1, GL_FALSE, &currentMatrix[0][0]);
    }
}

// Stub GLUT functions
inline void glutSetWindow(int) {}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../SDKSRC/Base/CBaseV1_2.h"
#include "CANDIDATE.h"

#define kWindowWidth	(320)
#define kWindowHeight	(480)
//#include CANDIDATE




#define TIMERMSECS 33

int startTime;
int prevTime;

App app;
static int elapsedTime;

static void HandleDisplay( )
{
    app.Render(glutGet(GLUT_ELAPSED_TIME),0,0,0);
    glutSwapBuffers( );
}

static void animate(int value)
{
	// Set up the next timer tick (do this first)
    glutTimerFunc(TIMERMSECS, animate, 0);
    
	// Measure the elapsed time
	int currTime = glutGet(GLUT_ELAPSED_TIME);
	int timeSincePrevFrame = currTime - prevTime;
	elapsedTime = currTime - startTime;
    
	// ##### REPLACE WITH YOUR OWN GAME/APP MAIN CODE HERE #####
    
	// Rotate the triangle
	//rot = (ROTRATE / 1000) * elapsedTime;
    
	// ##### END OF GAME/APP MAIN CODE #####
    
	
    
	// Force a redisplay to render the new image
	glutPostRedisplay();
    
	prevTime = currTime;
}

static void HandleIdle() {
    glutPostRedisplay();
}

static void HandleReshape( const int width, const int height )
{

    if (width!=320) {
        glutReshapeWindow(320, 480);
        return;
    }
    
    
    
 if (height>0)										// Prevent A Divide By Zero By
 {
		//height=1;										// Making Height Equal One
	
    
	glViewport(0,0,width,height);						// Reset The Current Viewport
	app.input.pushI(CMD_SCREENSIZE, width , height);
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
    
	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
    
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();
 }
}



static void exitIt() {
    app.Deinit();
    exit(0);
}

static void HandleKeyBoard( const unsigned char inKey, const int inPosX, const int inPosY )
{
    char inKey1;
    if (inKey==27) {
        exitIt();
    }
    if (inKey>='A' && inKey<='Z') app.input.pushI(CMD_KEYDOWN, inKey-49, 0);
}

static void HandleSpecialKey( const int inKey, const int inPosX, const int inPosY )
{
    char inKey1;
}

static void HandleMouse( const int inButton, const int inState, const int inPosX, const int inPosY )
{
    if (inButton==0) app.input.pushI(inState==0?CMD_TOUCH_START:CMD_TOUCH_END, inPosX, inPosY);
}

static void HandleMotion( const int inPosX, const int inPosY )
{
    int x = inPosX;
    int y = inPosY;
    app.input.pushI(CMD_TOUCHMOVE, x, y);
}

char ResPath[MAX_PATH];

void FindAppName() {
	FILE* f = fopen("CANDIDATE.h", "r");
	if (f)
	{
		char line[256];
        
		for (int i=0; i<100; i++) {
			fscanf(f, "%s", line);
			char* pn = strstr(line, ".hpp");
			strcpy(ResPath, line);
			if (pn) {
				int l = strlen(ResPath);
				ResPath[l-8]=0;
				strcat(ResPath, "/Data");
				break;
			}
		}
        
		fclose(f);
	} else {
        
    }
}

int main( int argc, char** argv )
{
    glutInit( &argc, argv );
    
    glutInitWindowSize( kWindowWidth, kWindowHeight );
    
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
    
    int theWindowHandle = glutCreateWindow( "AcnodeLabs 3D Planets" );
    glutSetWindow( theWindowHandle );
    
    glutDisplayFunc( HandleDisplay );
    glutReshapeFunc( HandleReshape );
    glutIdleFunc( HandleIdle );
    glutKeyboardFunc( HandleKeyBoard );
    glutSpecialFunc( HandleSpecialKey );
    glutMouseFunc( HandleMouse );
    glutMotionFunc( HandleMotion );
    HandleMotion( 0, 0 );
    
    // Setup default render states
    glClearColor( 0.3f, 0.3f, 0.3f, 1.0 );
    glEnable( GL_DEPTH_TEST );
    //glEnable( GL_COLOR_MATERIAL );
    
    FILE* f = fopen("CANDIDATE.h", "w");
    fputs("/Users/Bilal/Documents/REPOSITORY/ALGE-R1.1103/Apps/Planets/App.hpp", f);
    fclose(f);
    
    FindAppName();
    app.Init(ResPath);
    HandleReshape(kWindowWidth, kWindowHeight);
    
    startTime = glutGet(GLUT_ELAPSED_TIME);
    glutMainLoop( );
    
    return 0;
}
