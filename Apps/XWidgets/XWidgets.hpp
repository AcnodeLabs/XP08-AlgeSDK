// Copyright (c) 2018 AcnodLabs Inc
//Clone of GoDot's Example found at https://docs.godotengine.org/en/3.1/getting_started/step_by_step/your_first_game.html
//////////////////////////////XTimer
// on device : respath = /data/user/0/com.acnodelabs.DodgeTheCreeps/files

using namespace ImGui;

#include <string>
#include <sstream>

// Known Vendor IDs
#define VENDOR_APPLE     0x05AC
#define VENDOR_KEYCHRON  0x3434

// Keyboard Related:
//      SETUP_KEYEVENT_TAP :
// Next Slice:

class /*XWidgets*/ App : public AlgeApp {
	XButton btn_one;
	XButton btn_two;
	XButton btn_test;

public:

	string msg;

	virtual void onWidgetEvent(GameObject* sender, PEG::CMD* p) {
		if (btn_one.isYours(sender)) {
			msg = "You Clicked First Button";
		}
		if (btn_two.isYours(sender)) {
			msg = "You Clicked Second Button";
		}
		if (btn_test.isYours(sender)) {
			msg = "You Clicked Test Button";
		}
	}
    
    

   

    virtual bool onEventTap() {
        
        return true;
        
    }
    
	virtual void Init(char* path) {
	    AlInit(STANDARD);
		SetTitle("XWidgets");
		AddDefaultCamera(Camera::CAM_MODE_2D, OrthoTypes::ORIGIN_IN_TOP_LEFT_OF_SCREEN);

		btn_test.LoadIn(this, "Webpage"); // jpeg=true

		btn_one.LoadIn(this, "play", true); //or use LoadInEx(this, "btn_play_"); LoadIn Uses files in .btn folder with no prefix, LoadInEx User same folder and u have to provide prefix = "btn_play_"
		btn_two.LoadIn(this, "play", true); //or use LoadInEx(this, "btn_play_"); 

		btn_one.setPos(f3(100,100,0));
		btn_two.setPos(f3(300, 100, 0));
		btn_test.setPos(f3(100, 300, 0));

        Enable(KEYEVENT_TAP);
        
	}

	bool hover1, hover2;

    string detected_keyboards;
    
    std::ostringstream getDeviceDescription(unsigned int vendorId, unsigned int productId) {
        std::ostringstream oss;

        if (vendorId == VENDOR_APPLE) {
            oss << "Apple Keyboard";
        } else if (vendorId == VENDOR_KEYCHRON) {
            oss << "Keychron Keyboard";
        } else {
            oss << "Unknown Device";
        }
        
        oss << " (Vendor ID: 0x" << std::hex << vendorId
        << ", Product ID: 0x" << std::hex << productId << ")";
        
        if (keyspec_productId==productId) oss << "<<";
        
        return oss;
    }

    string keyspec_desc;
    char keyspec_keycode;
    int keyspec_productId;
    int keyspec_vendorId;
    
    virtual bool onKeySpec(int vendorId, int productId, int keycode) {
        printf("onKeySpec(0x%llx,0x%llx,%c", vendorId, productId, keycode);
        keyspec_productId = productId;
        keyspec_vendorId = vendorId;
        keyspec_keycode = keycode;
        printf(");\n");
        return true;
    }
    
	void processInput(PEG::CMD* p, float deltaT) {
		btn_one.processInput(p, deltaT);		
		btn_two.processInput(p, deltaT);
		btn_test.processInput(p, deltaT);

        if (p->command==DETECTED_KEYBOARD) {
            detected_keyboards = detected_keyboards + "\n" + getDeviceDescription(p->i1, p->i2).str();
        }
        
        if (p->command==CMD_KEYSPEC) {

        }
        
	}

    
    std::stringstream ss;
    
    char tgt_appTitle[32] = "TextEdit";
    std::string tgt_keyStrokes = " ";
    
    
    
    void RenderGui(float deltaT) {
		InfGuiHover();
		
        //	imHelper.ShowFPS(getBackgroundSize(), msg + " fps:", deltaT, f3(1., 1., 0.));
		
        ImGui::Begin("Detected Keyboards");
        keyspec_desc = to_string(keyspec_productId) + ":" + to_string(keyspec_keycode);
        
        //ss.clear();
        //ss << std::hex << keyspec_productId << ":" << keyspec_keycode;
        //std::string keyspec_desc = ss.str();
        
        
        tgt_keyStrokes[0] = keyspec_keycode;
        ImGui::Text("%s\n", keyspec_desc.c_str());
        ImGui::Text("%s", detected_keyboards.c_str());
        string btntext = "Title="+string(tgt_appTitle)+",key="+tgt_keyStrokes;
        ImGui::InputText("Target App", tgt_appTitle, 32);
        if (ImGui::Button(btntext.c_str())) {
            output.pushP(CMD_SENDKEYTO_APP, (void*)tgt_appTitle,  (void*)tgt_keyStrokes.c_str());
        }
        
        ImGui::End();
    }

	void UpdateCustom(GameObject* gob,int instanceNo, float deltaT) {

	}

};

