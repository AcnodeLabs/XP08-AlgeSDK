//#include <CBaseV1_2.h>
// Copyright (c) 2018 AcnodLabs Inc

/*
   ALGE SDK JD4 Demo :: XFs
   http://lib.ivank.net/demos/box2D.html
*/
using namespace ImGui;

#include "edu.lmu.cs/fly.cpp"
#include "edu.lmu.cs/landscape.cpp"
//#include "AdaCode/src/c_exports.h"

class /*XFs*/ App : public AlgeApp {

    GameObject openfly;
    GameObject gui;
    GameObject land;

public:
    
    virtual i2 getBackgroundSize() {
        return i2(1200, 900);
    }

    i2 getResolutionForGuiUsage() {
        return getBackgroundSize();
    }

    bool my_tool_active = true;
    float my_color[4];
    float roll, pitch, yaw;

    OPoint dp;

    void MyFirstToolWindow(float dt) {
        // Create a window called "My First Tool", with a menu bar.
        Begin("XFs Flight Sim", &my_tool_active, ImGuiWindowFlags_MenuBar);

        // Edit a color (stored as ~4 floats)
        //ColorEdit4("Color", my_color);

        // Plot some values
        const float my_values[] = { 0.2f, 0.1f, 1.0f, 0.5f, 0.9f, 2.2f };
        PlotLines("Frame Times", my_values, IM_ARRAYSIZE(my_values));

        // Display contents in a scrolling region
        TextColored(ImVec4(1,1,0,1), "Important Stuff");
        BeginChild("Scrolling");
        ImGui::Text("FPS %.2f", 1.0/dt);
        OPoint p = theShip.getPosition(); 
        Vector d = theShip.getDirection();

        ImGui::Text("Ship@ %.2f,%.2f,%.2f %.3f", p.x,p.y,p.z, theShip.speed);
        /*ImGui::SliderFloat("Speed", (float*)(&(theShip.speed)), -1.0f, 1.00f);
        ImGui::SliderFloat("Roll", (float*)(&(roll)), -100.0f, 100.00f);
        ImGui::SliderFloat("Pitch", (float*)(&(pitch)), -100.0f, 100.00f);
        ImGui::SliderFloat("Yaw", (float*)(&(yaw)), -100.0f, 100.00f);       
        */
        ImGui::SliderFloat("Speed", (float*)(&(theShip.speed)), -1.0f, 1.00f);
        ImGui::SliderFloat("dp_X", (float*)(&(dp.x)), -100.0f, 100.00f);
        ImGui::SliderFloat("dp_y", (float*)(&(dp.y)), -100.0f, 100.00f);
        ImGui::SliderFloat("dp_z", (float*)(&(dp.z)), -100.0f, 100.00f);
        ImGui::Text("Pos@ %.2f,%.2f,%.2f %.3f", p.x, p.y, p.z, theShip.speed);
        ImGui::Checkbox("Wireframe", &wireframe);
        aCamera.SetPos(f3(dp.x,dp.y,dp.z));

        EndChild();
        End();
    }
    int force;
   
    void RenderGui(float dt) {
        if (gui.Visible()) {
            GuiStarts();
                MyFirstToolWindow(dt);
            ImGui::ShowDemoWindow();
            GuiEnds();
        }
    }
    f3 up_ijk;


    void UpdateCustom(GameObject* gob,int instanceNo, float deltaT) {
        if (gob->is(aCamera)) {
            aCamera.Update(deltaT, gob);
        }
        if (gob->is(gui)) RenderGui(deltaT);
        if (gob->is(openfly)) {
            
         //   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       //     cockpit.draw();
            OPoint eye(theShip.getPosition());
            OPoint at(theShip.getPosition() + theShip.getDirection());
            Vector up(theShip.getVertical());
            aCamera.SetPos(eye.x, eye.y, eye.z);
            theShip.roll(roll); theShip.pitch(pitch); theShip.roll(yaw);
            Vector upv = theShip.getVertical();
            up_ijk.x= upv.i;
            up_ijk.y = upv.j;
            up_ijk.z = upv.k;
            //aCamera.rot.x = up_ijk.x;
            //aCamera.rot.y = up_ijk.y;
            //aCamera.rot.z = up_ijk.z;
            //aCamera.ViewFromCurrent();
         //   land.SetPos(at.x, at.y, at.z);//We are looking AT THE LAND
            glFlush();
            openfly_display();
            theShip.fly();
            

           

          //  glLoadIdentity();
         //   aCamera.SetPos(eye.x, eye.y, eye.z);
            
        }

    }

	virtual void processInput(PEG::CMD* p, float deltaT) {
        
        if (p->command == CMD_KEYDOWN) {
            if (p->i1 == ' ') gui.hidden = !gui.hidden;
            keyboard(p->i1,0,0);
            aCamera.processKeyDownWin(p->i1, deltaT);
        }
        
        
        if (p->command == CMD_TOUCH_START) {
            gui.hidden = false;
        }
	}

	virtual void Init(char* path) {
        
		AlInit(STANDARD_2D, "XFs Flight Sim");
		AddDefaultCamera(Camera::CAM_MODE_LOOKAT, OrthoTypes::ORIGIN_IN_MIDDLE_OF_SCREEN);
	//	InitPhysics();
    //    AddResource(&gui, "gui");  //Order of render gui matters on mac but not on windows Why??????
	//	AddResource(&skydome, "skydome", "skydome.jpg", XFunction_AutoScalingToFullScreen::AUTO_SCALING_FULLSCREEN);
       
       // gui.Hide();
        
        openfly_init();
        AddResource(&openfly, "openfly");
        AddResource(&land, "land");
        selectedObject = &land;
        
//        int j = ada_my_func(2);
        AddResource(&gui, "gui");

	}
};
