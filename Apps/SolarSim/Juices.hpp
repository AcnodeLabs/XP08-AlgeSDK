// Copyright (c) 2018 AcnodLabs Inc

/* 
   ALGE SDK JD3 Demo :: Juices
   “A juicy game feels alive and responds to everything you do – 
   tons of cascading action and response for minimal user input. 
   It makes the player feel powerful and in control of the world, 
   and it coaches them through the rules of the game by constantly 
   letting them know on a per-interaction basis how they are doing.”
   (Quoted from https://bit.ly/2A27Ke4)
*/
using namespace ImGui;

class /*Juices*/ App : public AlgeApp { 
	

	GameObject objct;
	GameObject background;
	PosRotScale *sel;
	
	XButton btn_play[8][8];

	int currentJuice[2];

public:

		virtual void Init(char* path) {
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
