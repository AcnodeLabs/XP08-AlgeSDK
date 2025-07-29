 // Copyright (c) 2018 AcnodLabs Inc

/* 
   ALGE SDK JD3 Demo :: XGui
*/

//App Specifics
#include "AnotherOne.hpp"
#include "MauiReader.hpp"
#include "XDefaultMauiPage.hpp"

#include "../XGui.Assets/Data/index.hpp"
#include "TextEditor.hpp"


using namespace AppLogic;

class /*XGui*/ App : public AlgeApp {
   // IceTop icetop;
	GameObject background, gui;
    string spath;
    IceTop icetop;
public:
    TextEditor ted;
    GameObject cloud, gmap;
    MauiReader xaml;

    App() {}
    
    void updateEditor() {
        std::string xamlcontent(xaml.duffer.begin(), xaml.duffer.end());
        ted.SetText(xamlcontent);
    }

    void GuiPage_Load(GameObject* gob_gui) {
    
        xaml.Init(("../../../XalApps/Maui"), this)->Load("VehIsb/MainPage.xaml",true/*HOTRELAOD*/);

       // xaml.Init(("../../../XalApps/Maui"), this)->Load("VehIsb/TestFont.xaml");
        AddResource(&cloud, "cloud", "cloud.jpg", 1.0)->hidden = true;
        AddResource(&gmap, "map_snap", "map_snap.jpg", 1.0)->hidden = true;
        AddObject(&gui);
        icetop.proj = "scripts";
        string dart = icetop.Get("/scripts/icetop/icetop.dart");
        int d = dart.length();
        updateEditor();
    }

    //Primary Functions
    virtual void Init(char* path) {
        spath = (string(path));
        AlInit(STANDARD);
        AddDefaultCamera(Camera::CAM_MODE_2D,OrthoTypes::ORIGIN_IN_TOP_LEFT_OF_SCREEN);
        
        AddResource(&background, "juices","green_natural.jpg",
                XFunction_AutoScalingToFullScreen::AUTO_SCALING_FULLSCREEN
        );
                
        GuiPage_Load(&gui);
        gui.SetJuiceScaleIn();
        
    }
    
    i2 getResolutionForGuiUsage() {
        return size_note10plus.half();
    }

    string formatFromLibResult(string x)// x is xml based
    {
        vector<string> xx = split(x, '>');
        if (xx.size() < 86) return "No Record Found";
        int i = xx[0].find("a href") != string::npos ? 2 : 0;
        string ret =
            "Status    : " + split(xx[i+85],'<')[0] + "\n" +
            "Paid Upto : " + split(xx[i + 79],'<')[0] + "\n" +
            "Owner No  : " + split(xx[i + 67],'<')[0] + "\n" +
            "Make      : " + split(xx[i + 31],'<')[0] + "\n" +
            "Year      : " + split(xx[i + 37],'<')[0] + "\n" +
            "Color     : " + split(xx[i + 55],'<')[0] + "\n" +
            "Owner     : " + split(xx[i + 73],'<')[0] + "\n" +
            "Category  : " + split(xx[i + 19],'<')[0] + "\n" +
            "Body      : " + split(xx[i + 25],'<')[0] + "\n" +
            "Engine CC : " + split(xx[i + 61],'<')[0] + "\n" +
            "Chassis No: " + split(xx[i + 43],'<')[0] + "\n" +
            "Engine No : " + split(xx[i + 49],'<')[0] + "\n" +
            "Date Registered  : " + split(xx[i + 14],'<')[0] + "\n" +
            "Lifetime Token   : " + split(xx[i + 93],'<')[0] + "\n" +
            "";
        ret = replaceAll(ret, "\\\"", "");
        return ret;
    }

    string correction(string lin) {
        string li = replaceAll(toUpperCase(lin),"-", " ");
        string nums = "";
        string alph = "";
        string ret = "";
        for (int i = 0; i < int(li.length()); i++) {
            char c = li[i];
            if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9') {
                nums = nums + c;
            }
            else {
                alph = alph + c;
            }
        }
        alph = trim(alph);
        return alph + '-' + nums;
    }

    int wx = 0;
    int wy = 0;

    void processInput(PEG::CMD* p, float deltaT) {
	    if (p->command == CMD_TOUCH_END) {
            if (xaml.isbtnpressed[0]) {
              xaml.labels[0]= formatFromLibResult(AppLogic::Bridge::onBtn(0, correction(xaml.inputtext[0])));
            }
        }
        if (p->command == CMD_TOUCHMOVE) {
            
        }
    }
  	
    bool firstRender = true;

    void UpdateCustom(GameObject* gob,int instanceNo, float deltaT) {
        if (gob->is(gui))
        {
            wx++;
            GuiStarts();
            coord.getGuiScale();
            ImGui::ShowDemoWindow();
            //ImGui::SetWindowPos(ImVec2(wx,wy));
        //    updateEditor();
            ted.Render("Editor", ImVec2(coord.getResolutionForGuiUsage().x, coord.getResolutionForGuiUsage().y), true);
            xaml.Render();
            GuiEnds();
        }

	}

};
