// Copyright (c) 2025 AcnodLabs Inc

/* 
   ALGE SDK Demo :: NeHeLesson05
*/

#define alInit AlInit
#define AL_KEY_UP KEY_FWD
#define AL_KEY_DOWN KEY_BAC
#define AL_KEY_LEFT KEY_LFT
#define AL_KEY_RIGHT KEY_RGT


namespace Jd2 {
#include "App.hpp"
};


class /**/ AppJd2 : public AlgeApp {
	Jd2::App app;

public:



	void Init(char* path) {
		app.Init(path);
	}

	void processInput(PEG::CMD* p, float deltaT) {
		
	}

	void RenderGui(float deltaT) {
		//	InfGuiHover();//Display Hover Information
		output.push(app.output.pull());
		app.input.push(input.pull());
		app.Render(deltaT, accel.x, accel.y, accel.z);
	}

	~AppJd2() {
		app.Deinit();
	}

};

class App : public AppJd2 {

};
