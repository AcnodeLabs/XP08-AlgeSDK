 // Copyright (c) 2018 AcnodLabs Inc

/* 
   ALGE SDK JD3 Demo :: XGui
*/

//App Specifics
#include "AnotherOne.hpp"
#include "MauiReader2.hpp"
#include "XDefaultMauiPage.hpp"

#include "../XGui.Assets/Data/index.hpp"
#include "TextEditor.hpp"


using namespace AppLogic;

class /*XGui*/ App : public AlgeApp {
   // IceTop icetop;
	GameObject background, gui;
 
    IceTop icetop;
public:
    TextEditor ted;
    GameObject cloud, map_snap, bot;
    MauiReader app1, toolbox;
    string proj = "C:/Users/liveb/source/repos/XalApps/Maui/VehIsb";

    void GuiPage_Load(GameObject* gob_gui) {
        app1.Init((proj), this)->Load("MainPage.xaml", false/*HOTRELAOD*/);
        
        AddResource(&bot, "dotnet_bot", "dotnet_bot.jpg", 1.0)->hidden = true;
        AddResource(&cloud, "cloud", "cloud.jpg", 1.0)->hidden = true;
        AddResource(&map_snap, "map_snap", "map_snap.jpg", 1.0)->hidden = true;
        AddObject(&gui);
        icetop.proj = proj;
    }

    //Primary Functions
    virtual void Init(char* path) {
        AlInit(STANDARD);
        AddDefaultCamera(Camera::CAM_MODE_2D,OrthoTypes::ORIGIN_IN_TOP_LEFT_OF_SCREEN);
        AddResource(&background, "juices","green_natural.jpg",
                XFunction_AutoScalingToFullScreen::AUTO_SCALING_FULLSCREEN
        );
        GuiPage_Load(&gui);
        toolbox.Init((string(path)), this)->Load("ToolBox.xaml", true/*HOTRELAOD*/);
    }
    
    
    i2 getResolutionForGuiUsage() {
        return size_a51.half().flip();
    }

    void processInput(PEG::CMD* p, float deltaT) {
	    if (p->command == CMD_TOUCH_END) {
            XGui::dumps(proj+"/MainPage.xaml");
        }
    }
  	
    bool firstRender = true;

    vector<XGui::Elem> u;

    void UpdateCustom(GameObject* gob,int instanceNo, float deltaT) {
        if (gob->is(gui))
        {
            GuiStarts();
       //     ImGui::SetNextWindowPos(ImVec2(0, 0));
            XGui::Begin("VehIsb.MainPage", "ISLAMABAD VEHICLE CHECK");
            
            {
                XGui::Elem e;
                e.xName = "txtPlate";
                e.Text = "JB-683";
                e.FontSize = "22";
                e.SemanticProperties.Description = "e.g AC201, jb123m JB 123 etc";
                e.Focused = "Entry_Focused";
                XGui::Entry(e);
            }
            {
                XGui::Elem e;
                e.xName = "btnFind";
                e.Text = "Find";
                e.SemanticProperties.Hint = "Search online db";
                e.Clicked = "OnFindButtonClicked"; 
                XGui::Button(e);
            }
            
            {
                XGui::Elem e;
                e.xName = "txtResult";
                e.Text = "Your Text will appear here";
                e.HeightRequest = "300";
                XGui::Label(e);
            }
            {
                XGui::Elem e;
                e.Source = "map_snap.jpg";
                e.HeightRequest = "200";
                XGui::Image(e, &rm);
            }
            {
                XGui::Elem e;
                e.Text = "Data Obtained From : "; 
                XGui::Label(e);
            }
            {
                XGui::Elem e;
                e.Source = "cloud.jpg";
                e.HeightRequest = "20";
                XGui::Image(e, &rm);
            }
            {
                XGui::Elem e;
                e.Text = "Peer Network";
                XGui::Label(e);
            }
            
            XGui::End();
       //     ImGui::SetNextWindowPos(ImVec2(200,200));
            toolbox.Render();

            GuiEnds();
        }

	}

};
