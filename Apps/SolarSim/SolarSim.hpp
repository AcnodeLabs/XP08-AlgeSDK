// Copyright (c) 2024 AcnodLabs Inc

/* 
   SOLAR SYSTEM CALC AND SIM
*/

#include <iostream>
#include <algorithm> // For std::max and std::min

using namespace ImGui;
using namespace std;

class SolarSim {
public:

	class Battery {
	public:
		struct specs {
			short AH; // Ampere-hours
			short V;  // Voltage
		} Reading;

		Battery() {}

		void setSpec(specs s) {
			Reading.AH = s.AH;
			Reading.V = s.V;
		}

		specs discharge(float amps, float dt) {
			float discharged_ah = amps * dt; // AH = A * hours
			Reading.AH = max(0, Reading.AH - static_cast<short>(discharged_ah));
			return Reading;
		}

		specs charge(float amps, float dt) {
			float charged_ah = amps * dt; // AH = A * hours
			Reading.AH = min(maxCapacity, Reading.AH + static_cast<short>(charged_ah));
			return Reading;
		}

		void setMaxCapacity(short max_ah) {
			maxCapacity = max_ah;
		}

	private:
		short maxCapacity; // Maximum capacity in AH
	};

	

	static int TestBattery() {
		ostringstream oss;
		Battery myBattery; // 100 AH, 12 V battery
		Battery::specs v12ah120;
		v12ah120.V = 12; v12ah120.AH = 120;
		myBattery.setSpec(v12ah120);
		myBattery.setMaxCapacity(0.8*v12ah120.AH); // Setting the max capacity to 100 AH

		oss << "Initial: " << myBattery.Reading.AH << " AH, " << myBattery.Reading.V << " V\n";
		netmsg.Post(oss.str());
		oss.clear();

		myBattery.discharge(10.0f, 9.0f); // Discharge at 10A for 1 hour
		oss << "After discharge: " << myBattery.Reading.AH << " AH, " << myBattery.Reading.V << " V\n";
		netmsg.Post(oss.str());
		oss.clear();

		myBattery.charge(10.0f, 9.0f); // Charge at 5A for 2 hours
		oss << "After charge: " << myBattery.Reading.AH << " AH, " << myBattery.Reading.V << " V\n";
		netmsg.Post(oss.str());
		oss.clear();

		return 0;
	}

};

class /*SolarSim*/ App : public AlgeApp { 
	

	SolarSim Sim;

	GameObject objct;
	GameObject background;
	PosRotScale *sel;
	
	XButton btn_play[8][8];

	int currentJuice[2];

public:

		virtual void Init(char* path) {

			Sim.TestBattery();
			return;
		//	wireframe = true;
		currentJuice[0] = 0;
		currentJuice[1] = 0;

		AlInit(STANDARD);
		SetTitle("Juices");
		output.pushP(CMD_TOAST, $ "Juices are Effects applicable to single game object or its instance\nTouch UP DOWN to change Object\nTouch RT LT to change Effects (Juices)", 0);
		
		AddDefaultCamera(Camera::CAM_MODE_2D);
	
		AddResource(&background, "juices",
			"green_natural.jpg",
			XFunction_AutoScalingToFullScreen::AUTO_SCALING_FULLSCREEN
		)->UUID = "Background";

		GameObject* lrtb = AddResource(&objct, "slice",0.25f);
		CRect r = lrtb->getOwnRect();
		objct.AddInstance(f2(rightSide*0.25, bottomSide/2))->UUID = "LEFT";
		objct.AddInstance(f2(rightSide*0.75, bottomSide/2))->UUID = "RIGHT";
		objct.AddInstance(f2(rightSide * 0.5, bottomSide * .25))->UUID = "UPPER";
		objct.AddInstance(f2(rightSide * 0.5, bottomSide * .75))->UUID = "LOWER";
		for (int rows = 0; rows<8; rows ++)
			for (int cols = 0; cols < 8; cols++) {
				btn_play[rows][cols].LoadIn(this, "play" /*Load Files from play.btn folder*/);
				btn_play[rows][cols].setPos(f3((rows+1) *50 *2, (cols)*50*2,0));
			}

	}
	
	string jn;
    
	void processInput(PEG::CMD* p, float deltaT) { 
		for (int rows = 0; rows < 8; rows++)
			for (int cols = 0; cols < 8; cols++) {
				btn_play[rows][cols].processInput(p, deltaT);
			}
	}
	

	void RenderGui(float dt) {

		InfGuiHover(dt, getPreferredScale());

		ImGui::Begin("JuiceTypes");
		SetWindowFontScale(getPreferredScale());
		static int e = 0;
		for (int i=1; i<JuiceTypes::JUICES_END;i++)
			ImGui::RadioButton((JuiceName(i)).c_str(), &e, i); ImGui::SameLine();
		ImGui::Text("-");
		objct.getInstancePtr(0)->JuiceType = e;
		objct.getInstancePtr(1)->JuiceType = e;
		objct.getInstancePtr(2)->JuiceType = e;
		objct.getInstancePtr(3)->JuiceType = e;

		ImGui::End();
	}


	PosRotScale* selected;

	bool demo = true;
	void UpdateCustom(GameObject* gob,int instanceNo, float deltaT) {
		
		if (gob->is(objct)) {
			PosRotScale* cur = objct.getInstancePtr(instanceNo);
		//	cur->ShowVitals();
		}
			
	}

};
