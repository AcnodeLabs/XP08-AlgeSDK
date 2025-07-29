 // Copyright (c) 2018 AcnodLabs Inc

/* 
   ALGE SDK JD3 Demo :: XGui
*/

//App Specifics
#include "AnotherOne.hpp"
#include "MauiReader.hpp"
#include "XDefaultMauiPage.hpp"

#include "../XGui.Assets/Data/index.hpp"

using namespace AppLogic;

class /*XGui*/ App : public AlgeApp {
    
	GameObject background, gob_gui, dotnet_bot;
    XGUI_App_Windows app_gui;
    MauiReader xaml;
    string spath;
    
public:
   
    //Primary Functions
    virtual void Init(char* path) {
        spath = (string(path));

        xaml.Init(string("../../../XalApps/Maui"),this)->Load("HomeFoodAround/HomeFoodAround/MainPage.xaml");


        AlInit(STANDARD);
            
        AddDefaultCamera(Camera::CAM_MODE_2D,OrthoTypes::ORIGIN_IN_TOP_LEFT_OF_SCREEN);
        
        AddResource(&background, "juices",
                "green_natural.jpg",
                XFunction_AutoScalingToFullScreen::AUTO_SCALING_FULLSCREEN
        );

        AddResource(&gob_gui, "gui");
        
        AddResource(&dotnet_bot, "dotnet_bot", "dotnet_bot.jpg",1.0)
            ->hidden= true;
        
        AppLogic::Bridge::Init(this);
        coord.setOrientation(LANDSCAPE);
        app_gui.Init(string(path));       
    }
    
    i2 MauiDefaultAppSize = i2(1426, 836);

    i2 getResolutionForGuiUsage() {

        return MauiDefaultAppSize;
    }

    void processInput(PEG::CMD* p, float deltaT) {
		if (p->command == CMD_SCREENSIZE) app_gui.msize = i2(p->i1,p->i2);	
        if (p->command == CMD_TOUCH_END) {
            for_i(20) {
                if (xaml.isbtnpressed[i]) {
                    AppLogic::Bridge::onBtn(i);
                    xaml.labels[0] = AppLogic::Logic::getAnswer();
                }
            }
            _for
        }
  	}

    bool firstRender = true;

	void UpdateCustom(GameObject* gob,int instanceNo, float deltaT) {
        if (gob->is(gob_gui))
        {
            GuiStarts();
                app_gui.ShowWindows(deltaT);
                xaml.Render();
            GuiEnds();
        }

	}

    //Secondary Functions
    i2 getBackgroudSize() {return app_gui.msize;}
};
