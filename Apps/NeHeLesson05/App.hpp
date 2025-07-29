// Copyright (c) 2012 AcnodLabs Inc

/* 
ALGE SDK Demo :: NeHeLesson05 (coolness added)
*/

#include "../../../Base/cbasev1_2.h"

class CShaderManager {
public:
	char * my_fragment_shader_source;
	char * my_vertex_shader_source;
	
	GLenum my_program;
	GLenum my_vertex_shader;
	GLenum my_fragment_shader;

	void SetupAndUse() {
		// Create Shader And Program Objects
		//my_program = glCreateProgramObjectARB();
		//my_vertex_shader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
		//my_fragment_shader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

		//// Load Shader Sources
		//glShaderSourceARB(my_vertex_shader, 1, &my_vertex_shader_source, NULL);
		//glShaderSourceARB(my_fragment_shader, 1, &my_fragment_shader_source, NULL);

		//// Compile The Shaders
		//glCompileShaderARB(my_vertex_shader);
		//glCompileShaderARB(my_fragment_shader);

		//// Attach The Shader Objects To The Program Object
		//glAttachObjectARB(my_program, my_vertex_shader);
		//glAttachObjectARB(my_program, my_fragment_shader);

		//// Link The Program Object
		//glLinkProgramARB(my_program);

		//// Use The Program Object Instead Of Fixed Function OpenGL
		//glUseProgramObjectARB(my_program);;
	}

};

class /*NeHeLesson05*/ App { 

	float rot;

public:
    CResourceManager rm;
	CShaderManager sm;
		
	PEG output;
	PEG input;
	aL10 AL10;

	float t0;

	void Render(float dt, int aX, int aY, int aZ) {
		t0+=dt;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
		glLoadIdentity();									// Reset The Current Modelview Matrix
		
		glTranslatef(0.0f,0.0f,-6.0f + (2.0f * sin(t0)));	
		glRotatef(rot,0.0f,1.0f,0.0f);	

		AL10.Capture(GL_TRIANGLES);							// Start Drawing A Triangle
		AL10.glColor3f(1.0f,0.0f,0.0f);						// Red
		AL10.glVertex3f( 0.0f, 1.0f, 0.0f);					// Top Of Triangle (Front)
		AL10.glColor3f(0.0f,1.0f,0.0f);						// Green
		AL10.glVertex3f(-1.0f,-1.0f, 1.0f);					// Left Of Triangle (Front)
		AL10.glColor3f(0.0f,0.0f,1.0f);						// Blue
		AL10.glVertex3f( 1.0f,-1.0f, 1.0f);					// Right Of Triangle (Front)
		AL10.glColor3f(1.0f,0.0f,0.0f);						// Red
		AL10.glVertex3f( 0.0f, 1.0f, 0.0f);					// Top Of Triangle (Right)
		AL10.glColor3f(0.0f,0.0f,1.0f);						// Blue
		AL10.glVertex3f( 1.0f,-1.0f, 1.0f);					// Left Of Triangle (Right)
		AL10.glColor3f(0.0f,1.0f,0.0f);						// Green
		AL10.glVertex3f( 1.0f,-1.0f, -1.0f);				// Right Of Triangle (Right)
		AL10.glColor3f(1.0f,0.0f,0.0f);						// Red
		AL10.glVertex3f( 0.0f, 1.0f, 0.0f);					// Top Of Triangle (Back)
		AL10.glColor3f(0.0f,1.0f,0.0f);						// Green
		AL10.glVertex3f( 1.0f,-1.0f, -1.0f);				// Left Of Triangle (Back)
		AL10.glColor3f(0.0f,0.0f,1.0f);						// Blue
		AL10.glVertex3f(-1.0f,-1.0f, -1.0f);				// Right Of Triangle (Back)
		AL10.glColor3f(1.0f,0.0f,0.0f);						// Red
		AL10.glVertex3f( 0.0f, 1.0f, 0.0f);					// Top Of Triangle (Left)
		AL10.glColor3f(0.0f,0.0f,1.0f);						// Blue
		AL10.glVertex3f(-1.0f,-1.0f,-1.0f);					// Left Of Triangle (Left)
		AL10.glColor3f(0.0f,1.0f,0.0f);						// Green
		AL10.glVertex3f(-1.0f,-1.0f, 1.0f);					// Right Of Triangle (Left)
		AL10.Flush(0, false, true, false);	

		rot+=0.8f;											
	}

	void Init(char* path) {
		alInit(STANDARD);
		output.pushP(CMD_TITLE, $ "Alge Nehe Lesson05",0);
		rot = 0;
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		t0= 0;
		glClearColor(COLOR_CORNFLOWERBLUE);
	}

	void Deinit() {
	}

};
