 // Copyright (c) 2018 AcnodLabs Inc

class /*XLaunch*/ App : public AlgeApp {
   // IceTop icetop;
	GameObject background, gui;
    string spath;
    IceTop icetop;
public:
    GameObject cloud, gmap;
   
    //Primary Functions
    virtual void Init(char* path) {
        spath = (string(path));
        AlInit(STANDARD);
        AddDefaultCamera(Camera::CAM_MODE_2D,OrthoTypes::ORIGIN_IN_TOP_LEFT_OF_SCREEN);
        
        AddResource(&background, "juices","green_natural.jpg",
                XFunction_AutoScalingToFullScreen::AUTO_SCALING_FULLSCREEN
        );
                
        gui.SetJuiceScaleIn();
    }
    
    
    i2 getResolutionForGuiUsage() {
        return size_a51.half().flip();
    }

    void processInput(PEG::CMD* p, float deltaT) {
	    if (p->command == CMD_TOUCH_END) {

        }
        if (p->command == CMD_TOUCHMOVE) {
            
        }
    }
  
    void UpdateCustom(GameObject* gob,int instanceNo, float deltaT) {
        if (gob->is(gui))
        {
            GuiStarts();
               // if (currentView) currentView();

            GuiEnds();
        }

	}

};
