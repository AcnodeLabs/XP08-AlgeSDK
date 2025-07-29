// Copyright (c) 2025 AcnodLabs Inc

/* 
   ALGE SDK Demo :: NeHeLesson03
*/

#define alInit AlInit

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
