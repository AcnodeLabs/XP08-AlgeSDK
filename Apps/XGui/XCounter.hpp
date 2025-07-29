// Copyright (c) 2018 AcnodLabs Inc

/* 
   ALGE SDK JD3 Demo :: XGui
*/

//App Specifics
#include "XCounterGui.hpp"

#include "../XGui.Assets/Data/index.hpp"

extern ImHelper imHelper;

class /*XCounter*/ App : public AlgeApp {
    GameObject bg, btn;
	GameObject gob_gui;
    XCounterGui app_gui;

public:

    App() {
        wireframe = false;
        coord.setOrientation(0);
    }

    virtual void Init(char* path) {
        AlInit(STANDARD);
        AddDefaultCamera(Camera::CAM_MODE_2D);
        AddResource(&bg, "juices", "bg_blue.jpg",       XFunction_AutoScalingToFullScreen::AUTO_SCALING_FULLSCREEN
        );
     
        app_gui.Init(string(path), coord.getResolutionForGuiUsage(), this);

        AddResource(&gob_gui, "gui");

        AddResource(&btn, "start", "start", 1);

    }
    
    short m_currHr = -1;
    short m_day = -1;
    short m_month = -1;  

    void processInput(PEG::CMD* p, float deltaT) {

        if (p->command == CMD_PAUSE || p->command == CMD_END) {
            app_gui.SaveArr();
        }

        if (p->command == CMD_SYSTEMDATE) {
            short* ptr = (short*)(p->param1);
            m_day = *ptr;
            m_month = *(ptr+1);
            int m_yr = *(ptr+ 2);
        }

        if (p->command == CMD_MMDD) {
            m_month = p->i1;
            m_day = p->i2;
        }
        
        if (p->command == CMD_HHMM) {
            
            if (m_currHr == -1) {
                m_currHr = p->i1;
                app_gui.onAppOpen(m_month, m_day, p->i1);
            }

            if (p->i1 == 0 && p->i1 != m_currHr) {
                app_gui.onDayChanged(m_day);
                
            }
            if ((m_currHr+1) == p->i1)
                app_gui.onHourChanged(m_currHr);
            
            m_currHr = p->i1;
            
        }
  	}



	void UpdateCustom(GameObject* gob,int instanceNo, float deltaT) {

        if (gob->is(btn)) {
            string name = gob->UUID;
        }

        if (gob->is(gob_gui))
        {
            GuiStarts();
             i2 s = coord.getResolutionForGuiUsage();
                ImGui::SetNextWindowPos(ImVec2(0,0));
                ImGui::SetNextWindowSize(ImVec2(s.x,s.y));
                app_gui.ShowWindows(deltaT,1);
                bg.rot.x = app_gui.fault* 10;
                bg.rot.y = app_gui.fault * 10;
       //         bg.rot.z = app_gui.fault * 10;
            GuiEnds();
        }

	}
};
