 // Copyright (c) 2018 AcnodLabs Inc

/* 
   ALGE SDK JD3 Demo :: XGui
*/

//App Specifics
#include "AnotherOne.hpp"
#include "../../../XCOMMONS/cpp/reuse/reuse.hpp"
#include "MauiReader2.hpp"
#include "XDefaultMauiPage.hpp"

#include "../XGui.Assets/Data/index.hpp"
#include "TextEditor.hpp"


using namespace AppLogic;

class /*XamlGen2*/ App : public AlgeApp {
   // IceTop icetop;
	GameObject background, gui;
 
    IceTop icetop;
public:
    TextEditor ted;
    GameObject cloud, map_snap, bot, vert, horiz;
    MauiReader app1, toolbox;
    string basedir = "C:/Users/liveb/source/repos/MauiCalc/MauiCalc";//default example  // loaded from mauiproj.text first line
    string pagename = "MainPage.xaml";//default example  // loaded from mauiproj.text second line

    void GuiPage_Load(GameObject* gob_gui) {        
        AddResource(&bot, "dotnet_bot", "dotnet_bot.jpg", 1.0)->hidden = true;
        AddResource(&cloud, "cloud", "cloud.jpg", 1.0)->hidden = true;
        AddResource(&map_snap, "map_snap", "map_snap.jpg", 1.0)->hidden = true;
        AddResource(&vert, "vert", "vert.jpg", 1.0)->hidden = true;
        AddResource(&horiz, "horiz", "horiz.jpg", 1.0)->hidden = true;
        AddObject(&gui);
        icetop.proj = basedir;
    }

    void processCurDef(string def) {
        XGui::Elem a;
        a.fillfromXGTformat(def);
        u.push_back(a);
    }
    
    int ub = 0, ul = 0, ue = 0, ui = 0;
    string uf2egt(string ull) {
        string ret = "";
        
        for (auto u : ull) {
            if (u == 'b') ret += "ty:btn,na:btn" + to_string(ub) + ",tx=btn" + to_string(ub++) +",cl=OnCounterClicked" + "\n";
            if (u == 'l') ret += "ty:lbl,na:lbl"+to_string(ul)+",tx=lbl"+to_string(ul++) + "\n";
            if (u == 'e') ret += "ty:ent,na:ent" + to_string(ue) + ",tx=ent" + to_string(ue++) + "\n";
            if (u == 'i') ret += "ty:img,so:dotnet_bot.png,hr=100\n";
            if (u == 'm') ret += "ty:map,so:dotnet_bot.png,hr=300\n";
        }
        return string(trim(ret));
    }

    void microformat(string uf) {
        vector<string> uflines = split(uf, '\\n');
        u.clear(); curdefs.clear(); ub = 0; ul = 0; ue = 0; ui = 0;
        curdefs.push_back("|Vert_Begin|");
        for (auto ul : uflines) {
            curdefs.push_back("|Horiz_Begin|");
            curdefs.push_back(uf2egt(ul));
            curdefs.push_back("|Horiz_End|");
        }
        curdefs.push_back("|Vert_End|");

        for (auto def : curdefs) {
            processCurDef(def);
        }
    }

    void reloadCurdefs(string pagename) {
        curdefs.clear();
        u.clear();
        FILE* f = fopen((basedir + "/"+pagename+".xgt").c_str(), "r");
        char line[1024];
        if (f) {
            while (!feof(f)) {
                line[0] = 0;
                fgets(line, 1024, f);
                string_view tline = trim(line);
                if (line[0]) { curdefs.push_back(string(tline)); processCurDef(string(tline)); }
            }
            fclose(f);
        }
    }

    void parseXGT(string xamlfile) {
        app1.Init((basedir), this)->LoadXGT(xamlfile, false/*HOTRELAOD*/);
        reloadCurdefs(xamlfile);
    }

    //Primary Functions
    virtual void Init(char* path) {

        reuse::ConfigFile config("mauiproj.cfg");
        if (!config.load()) return;
        basedir = config.get("basedir");
        pagename = config.get("target");
        
        AlInit(STANDARD);
        AddDefaultCamera(Camera::CAM_MODE_2D,OrthoTypes::ORIGIN_IN_TOP_LEFT_OF_SCREEN);
        AddResource(&background, "juices","green_natural.jpg",
                XFunction_AutoScalingToFullScreen::AUTO_SCALING_FULLSCREEN
        );
        GuiPage_Load(&gui);
        
        parseXGT(pagename+".xaml");
    
        toolbox.Init((string(path)), this)->Load("ToolBox.xaml", true/*HOTRELAOD*/);
        
        return;
    }
    
    
    i2 getResolutionForGuiUsage() {
        return size_a51.half().flip();
    }
    short il, ib, ie, ii, im;

    /// <summary>
    /// exmple format :  so:bot.png,ho:Center,hr:50,wr:50
    /// </summary>
    /// <param name="e"></param>
    void setRequestedProperties(XGui::Elem &e, bool loading = false) {
        string props = toolbox.inputtext[0];
        if (!toolbox.inputtext[0][0]) {
            strcpy(toolbox.inputtext[0],"Example. ho:Center,hr:50,wr:50"); if (!loading) return;
        }
        e.fillfromXGTformat(props);
    }
    vector<string> curdefs;
    void add_elem(string ty, XGui::Elem e) {
        curdefs.push_back("ty:"+ty + "," + toolbox.inputtext[0]);
        u.push_back(e);
    }

    void processCurdefs() {
        int i = curdefs.size();
        for (auto d : curdefs) {
            string df = d;
            strcpy(toolbox.inputtext[0] , df.c_str());
            int iv = 0;
            if (df._Starts_with("ty:lbl")) {XGui::Elem e; e.xName = "lbl" + to_string(++il); setRequestedProperties(e,true); u.push_back(e); }
            if (df._Starts_with("ty:btn")) { XGui::Elem e; e.xName = "btn" + to_string(++ib); setRequestedProperties(e, true); u.push_back(e); }
            if (df._Starts_with("ty:img")) { XGui::Elem e; e.xName = "img" + to_string(++ii); 
            setRequestedProperties(e,true); u.push_back(e); }
            if (df._Starts_with("ty:ent")) { XGui::Elem e; e.xName = "ent" + to_string(++ie); setRequestedProperties(e, true); u.push_back(e); }
            if (df._Starts_with("|")) { XGui::Elem e; e.xName = df;  u.push_back(e); }

          /*  if (d) { XGui::Elem e; e.xName = "lbl" + to_string(++il); setRequestedProperties(e); add_elem(e); }
            if (toolbox.isbtnpressed[ibi++]) { XGui::Elem e; e.xName = "btn" + to_string(++ib); setRequestedProperties(e); add_elem(e); }
            if (toolbox.isbtnpressed[ibi++]) { XGui::Elem e; e.xName = "img" + to_string(++ii); setRequestedProperties(e); add_elem(e); }
            if (toolbox.isbtnpressed[ibi++]) { XGui::Elem e; e.xName = "ent" + to_string(++ie); setRequestedProperties(e); add_elem(e); }*/
        }
    }

    bool horizont = false;
    bool vertical = false;

    void onHori(string def) {
        XGui::Elem h;
        if (horizont == true) {
            h.xName = "|Horiz_Begin|";
        } else h.xName = "|Horiz_End|";
        curdefs.push_back(h.xName);
        u.push_back(h);
    }
    void onVert(string def) {
        XGui::Elem v;
        if (horizont == true) {
            v.xName = "|Vert_Begin|";
        }
        else v.xName = "|Vert_End|";
        curdefs.push_back(v.xName);
        u.push_back(v);
    }

    void processInput(PEG::CMD* p, float deltaT) {
	    if (p->command == CMD_TOUCH_END) {
            
            string m_page = app1.usernamespace + "." + pagename;
            string m_title = app1.m_xamlFile;
            short ibi = 0;
            bool dumpAsWell = false;//Dump Always
            if (toolbox.isbtnpressed[ibi++]) { if (u.size() > 0) u.pop_back(); curdefs.pop_back(); }
            if (toolbox.isbtnpressed[ibi++]) { u.clear();curdefs.clear(); }
            if (toolbox.isbtnpressed[ibi++]) { XGui::Elem e; e.xName = "lbl" + to_string(++il); setRequestedProperties(e); add_elem("lbl",e); }
            if (toolbox.isbtnpressed[ibi++]) { XGui::Elem e; e.xName = "btn" + to_string(++ib); setRequestedProperties(e); add_elem("btn", e); }
            if (toolbox.isbtnpressed[ibi++]) { XGui::Elem e; e.xName = "img" + to_string(++ii); setRequestedProperties(e); add_elem("img", e); }
            if (toolbox.isbtnpressed[ibi++]) { XGui::Elem e; e.xName = "ent" + to_string(++ie); setRequestedProperties(e); add_elem("ent",e); }
            if (toolbox.isbtnpressed[ibi++]) { XGui::Elem e; e.xName = "map" + to_string(++im); setRequestedProperties(e); add_elem("ent", e); }
            size_t n = u.size();
            if (toolbox.isbtnpressed[ibi++]) { horizont = !horizont; onHori(""); toolbox.labels[5] = horizont ? "ON" : "OFF"; }
            if (toolbox.isbtnpressed[ibi++]) { vertical = !vertical; onVert(""); toolbox.labels[6] = vertical ? "ON" : "OFF"; }
            if (toolbox.isbtnpressed[ibi++]) { 
                reloadCurdefs(pagename); dumpAsWell = true; 
            }
            if (toolbox.isbtnpressed[ibi++]) {
                microformat(toolbox.inputtext[0]); dumpAsWell =true;
            }
            if (dumpAsWell) 
                { 
                XGui::dumps(this, u, curdefs,m_page, m_title, app1.m_path + "/" + app1.m_xamlFile); 
                }
        }
    }
  	
    bool firstRender = true;

    vector<XGui::Elem> u;

    void UpdateCustom(GameObject* gob,int instanceNo, float deltaT) {
        if (gob->is(gui))
        {
            GuiStarts();
       //   ImGui::SetNextWindowPos(ImVec2(0, 0));
            string target = app1.usernamespace + "." + pagename;
            XGui::Begin(target, "Edit " + target);
            
            for (auto e : u) {
                if (e.xName._Starts_with("txt")
                    || e.xName._Starts_with("lbl")
                    ) XGui::Label(e);
                if (e.xName._Starts_with("btn")) {
                    bool b1 = XGui::Button(e);
                }
                if (e.xName._Starts_with("map")) XGui::Image(e, &rm);
                if (e.xName._Starts_with("img")) XGui::Image(e, &rm);
                if (e.xName._Starts_with("inp")
                    || e.xName._Starts_with("ent")) XGui::Entry(e);
                if (e.xName._Starts_with("|Horiz_Begin|")) 
                {
                    XGui::PushOrientation("horiz");
                    XGui::Elem e; e.xName = "imgHoriz"; e.Source = "horiz.jpg"; e.HorizontalOptions = "Start";  e.HeightRequest = "20"; XGui::Image(e, &rm);
                    
                }
                if (e.xName._Starts_with("|Vert_Begin|"))
                {
                    XGui::PushOrientation("vert");
                    XGui::Elem e; e.xName = "imgVert";  e.Source = "vert.jpg"; e.VerticalOptions = "Start"; ; e.HeightRequest = "20"; XGui::Image(e, &rm);
                   
                }
                if (e.xName._Starts_with("|Horiz_End|"))
                {
                    //XGui::Elem e; e.xName = "imgHoriz"; e.Source = "horiz.jpg"; e.HorizontalOptions = "Start";  e.HeightRequest = "20"; XGui::Image(e, &rm);
                    XGui::PopOrientation();
                }
                if (e.xName._Starts_with("|Vert_End|"))
                {
                    XGui::Elem e; e.xName = "imgVert";  e.Source = "vert.jpg"; e.VerticalOptions = "Start"; ; e.HeightRequest = "20"; XGui::Image(e, &rm);
                    XGui::PopOrientation();
                }

            }
            
            XGui::End();
       //     ImGui::SetNextWindowPos(ImVec2(200,200));
            toolbox.Render();

            GuiEnds();
        }

	}

};
