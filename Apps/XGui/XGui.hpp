// Copyright (c) 2018 AcnodLabs Inc

/* 
   ALGE SDK JD3 Demo :: XGui
*/

//App Specifics
#include "AnotherOne.hpp"

namespace XGUI_Json {
    static string text1 = "<Title>";
    static void process_json_xgui(char*) {};
}

#include <fstream>

class /*XGui*/ App : public AlgeApp {
    
	GameObject background, gob_gui;
    XGUI_App_Windows gui;
    
public:
    
    //Primary Functions
    virtual void Init(char* path) {
    
        XGUI_Json::process_json_xgui("{\"text1\": \"Start developing now\", \"text2\": \"Make changes to your XAML file and save to see your UI update in the running app with XAML Hot Reload.Give it a try!\"}");

        AlInit(STANDARD);
        SetTitle(XGUI_Json::text1.c_str());
    
        AddDefaultCamera(Camera::CAM_MODE_2D,OrthoTypes::ORIGIN_IN_TOP_LEFT_OF_SCREEN);
        
        AddResource(&background, "juices",
                "green_natural.jpg",
                XFunction_AutoScalingToFullScreen::AUTO_SCALING_FULLSCREEN
        );

        AddResource(&gob_gui, "gui");
        gui.Init(string(path));
        //guiFont.Init(string(path) + "/Roboto-Bold.ttf", 22.0f);
    }

    void processInput(PEG::CMD* p, float deltaT) {
		if (p->command == CMD_SCREENSIZE) gui.msize = i2(p->i1,p->i2);
        if (p->command == CMD_REMOTE_COMMAND) {
            char* tmx = (char*)&p->param1;
            string the_message = string(tmx);
            string tag = "XSVC_HTTP"; //
            if (the_message.find(tag) != std::string::npos) {
                gui.Set_mvars(the_message.substr(tag.length()));
            }
        }
  	}

	void UpdateCustom(GameObject* gob,int instanceNo, float deltaT) {
        if (gob->is(gob_gui))
        {
            GuiStarts();
                gui.ShowWindows(deltaT);
            GuiEnds();
        }

	}

    //Secondary Functions
    i2 getBackgroudSize() {return gui.msize;}
};
