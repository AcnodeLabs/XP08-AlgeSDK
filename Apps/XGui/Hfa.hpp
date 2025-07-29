 // Copyright (c) 2018 AcnodLabs Inc

/* 
   ALGE SDK JD3 Demo :: XGui
*/
#include <stack>
//App Specifics
#include "AnotherOne.hpp"
#include "MauiReader2.hpp"
#include "XDefaultMauiPage.hpp"
#include "Hfa_Views.hpp"
#include "../XGui.Assets/Data/index.hpp"

using namespace AppLogic;

class /*HFA*/ App : public AlgeApp {
   // IceTop icetop;
	GameObject background, gui;
    string spath;
    IceTop icetop;
public:
    GameObject cloud, gmap;
    MauiReader xHelpPage, xItemDetailScreen,xLoginPage,xManageScreen,xPrimaryScreen;

    stack<MauiReader*> navigate;

    void GuiPage_Load(GameObject* gob_gui, string datadir) {
        xHelpPage.Init(datadir,this)->Load("GuiPages/Hfa_Views/MainPage.xaml", true);//HOT RELOAD
        xItemDetailScreen.Init(datadir,this)->Load("GuiPages/Hfa_Views/ItemDetailScreen.xaml", true);
        xLoginPage.Init(datadir,this)->Load("GuiPages/Hfa_Views/LoginPage.xaml", true);
        xManageScreen.Init(datadir,this)->Load("GuiPages/Hfa_Views/ManageScreen.xaml", true);
        xPrimaryScreen.Init(datadir,this)->Load("GuiPages/Hfa_Views/PrimaryScreen.xaml", true);


        AddResource(&cloud, "cloud", "cloud.jpg", 1.0)->hidden = true;
        AddResource(&gmap, "map_snap", "map_snap.jpg", 1.0)->hidden = true;
        AddObject(&gui);
        icetop.proj = "scripts";
        string dart = icetop.Get("/scripts/icetop/icetop.dart");
        int d = dart.length();

        navigate.push(&xLoginPage);
    }

    //Primary Functions
    virtual void Init(char* path) {
        spath = (string(path));
        AlInit(STANDARD);
        AddDefaultCamera(Camera::CAM_MODE_2D,OrthoTypes::ORIGIN_IN_TOP_LEFT_OF_SCREEN);
        
        AddResource(&background, "juices","green_natural.jpg",
                XFunction_AutoScalingToFullScreen::AUTO_SCALING_FULLSCREEN
        );
                
        GuiPage_Load(&gui, spath);
        gui.SetJuiceScaleIn();
        currentView = LoginPage;
    }
    
    
    i2 getResolutionForGuiUsage() {
        return size_iphone5.half().flip();
    }

    void processInput(PEG::CMD* p, float deltaT) {
	    if (p->command == CMD_TOUCH_END) {
            if (xLoginPage.isButtonPressed(0)) {
                bool ok = doLogin();
                if (ok) navigate.push(&xPrimaryScreen);
            }
            if (xLoginPage.isButtonPressed(1)) {
                bool ok = doSignUp(); 
                if (ok) navigate.push(&xManageScreen);
            }
            if (xPrimaryScreen.isButtonPressed(1)) {
                navigate.push(&xItemDetailScreen);
            }
            if (xLoginPage.isButtonPressed(0)) navigate.push(&xHelpPage); 
            if (xHelpPage.isButtonPressed(0)) navigate.pop();
            if (xItemDetailScreen.isButtonPressed(0))
                navigate.pop();
        }
        if (p->command == CMD_TOUCHMOVE) {
            
        }
    }
  
    void UpdateCustom(GameObject* gob,int instanceNo, float deltaT) {
        if (gob->is(gui))
        {
            GuiStarts();
               // if (currentView) currentView();
                navigate.top()->Render();
            GuiEnds();
        }

	}

};
