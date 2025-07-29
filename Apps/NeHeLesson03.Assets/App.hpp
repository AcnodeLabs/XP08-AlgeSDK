// Copyright (c) 2012 AcnodLabs Inc

/* 
   ALGE SDK Demo :: NeHeLesson03
*/

#include "../../Base/CBaseV1_2.h"

class /*Lesson03*/ App { 

public:
    CResourceManager rm;
    
	PEG output;
	PEG input;
	aL10 AL10; // To Make Desktop OpenGL style calls on an ES Platform
	

	void Render(float dt, int aX, int aY, int aZ) {		
			
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
		glLoadIdentity();									// Reset The Current Modelview Matrix
		glTranslatef(-1.5f,0.0f,-10.0f);					// Move
		AL10.Capture(GL_TRIANGLES);							// Drawing Using Triangles
		 AL10.glColor3f(1.0f,0.0f,0.0f);						// Set The Color To Red
		 AL10.glVertex3f( 0.0f, 1.0f, 0.0f);					// Top
		 AL10.glColor3f(0.0f,1.0f,0.0f);						// Set The Color To Green
		 AL10.glVertex3f(-1.0f,-1.0f, 0.0f);					// Bottom Left
		 AL10.glColor3f(0.0f,0.0f,1.0f);						// Set The Color To Blue
		 AL10.glVertex3f( 1.0f,-1.0f, 0.0f);					// Bottom Right
		 AL10.Flush(0,false,true,false);					// Finished Drawing The Triangle
		glTranslatef(3.0f,0.0f,0.0f);						// Move Left 1.5 Units And Into The Screen 6.0
		glColor4f(0.5f,0.5f,1.0f, 1.0f);	 
		
		AL10.Capture(GL_TRIANGLES);							// Drawing Using Triangles
	     AL10.glVertex3f(-1.0f, 1.0f, 0.0f);					
	     AL10.glVertex3f( 1.0f, 1.0f, 0.0f);					
		 AL10.glVertex3f( 1.0f,-1.0f, 0.0f);					
		 AL10.glVertex3f( 1.0f,-1.0f, 0.0f);					
		 AL10.glVertex3f(-1.0f,-1.0f, 0.0f);	
		 AL10.glVertex3f(-1.0f, 1.0f, 0.0f);					
		AL10.Flush(0,false,false,false);
	}

	void Init(char* path) {
		alInit(STANDARD);
		output.pushP(CMD_TITLE, $ "Alge Nehe Lesson03",0);
		
	}

	void Deinit() {
	}


};
