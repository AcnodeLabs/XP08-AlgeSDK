// Copyright c) 2018 AcnodLabs Inc

#include "TextEditor.hpp"
#undef pi
#include "ImFileDialog.h"
#include "ImFileDialog.cpp"

/*
ifd::FileDialog::Instance().CreateTexture = [](uint8_t* data, int w, int h, char fmt) -> void* {
	GLuint tex;

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, (fmt == 0) ? GL_BGRA : GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	return (void*)tex;
	};
ifd::FileDialog::Instance().DeleteTexture = [](void* tex) {
	GLuint texID = (GLuint)tex;
	glDeleteTextures(1, &texID);
	};
*/

#include <fstream>

#define reusable static
class /**/ App : public AlgeApp {
	GameObject bg;
	GameObject logo;

	TextEditor ted;
	string datadir;

	
public:



	virtual i2 getBackgroundSize() {
		return size_dev_WUXGA_portrait.flip();
	}

	vector<string> availableFonts;
	string availableFontsNullSeparatedForImGuiUse;

	void Init(char* path) {
		
		AlInit(STANDARD);

		string names = "";
		availableFonts = GetFileNames(path, "ttf");
		
		
		for (auto a : availableFonts) {
			availableFontsNullSeparatedForImGuiUse += a.substr(string(path).size()) + '\0';
		}

		string loadedPreference = LoadPreference("Font");

		if (loadedPreference.length() <= 0) {
			if (availableFonts.size()>0)
				UseTtf((availableFonts[0]).c_str(), 20.0f);
		}
		else {
			UseTtf(loadedPreference.c_str(), 20.0f);
		}

		datadir = string(path);
		AddDefaultCamera(Camera::CAM_MODE_2D, OrthoTypes::ORIGIN_IN_TOP_LEFT_OF_SCREEN);
		AddResource(&bg, "techno", "techno.jpg",XFunction_AutoScalingToFullScreen::AUTO_SCALING_FULLSCREEN);
		AddResource(&logo, "1x1", "acnode.jpg", 100);
		logo.JuiceType = JuiceTypes::JUICE_PULSATE;
		SetTitle(apptitle.c_str());

	}

	string apptitle = "XText by Acnodelabs (c) 2024";
	string lastaction = "Ready";


	bool SaveEditorToFile(string filename) {
		fname = filename;
		if (filename.size() < 1) {
			lastaction = "Nothing Saved";
			return false;
		}
		//	MessageBoxA(NULL, filename.c_str(), "LoadFileToEditor", MB_ICONASTERISK);
		vector<string> lines = ted.GetTextLines(); 
		WriteFileContents(filename, lines);
		lastaction = to_string(lines.size()) + " Lines Saved";
		return (lines.size() > 0);
	}

	bool LoadFileToEditor(string filename) {
		fname = filename;
	//	MessageBoxA(NULL, filename.c_str(), "LoadFileToEditor", MB_ICONASTERISK);
		vector<string> lines = ReadFileContents(filename);
		ted.SetTextLines(lines);
		lastaction = to_string(lines.size()) + " Lines Read";
		return (lines.size() > 0);
	}

	int nArgs =0;

	string fname;
	void processInput(PEG::CMD* p, float deltaT) {
		if (p->command == CMD_KEYDOWN) {}
		if (p->command == CMD_TOUCH_START) {}
		if (p->command == CMD_SCREENSIZE) {
			width = p->i1;
		}
		if (p->command == CMD_HHMM) {}
		if (p->command == CMD_COMMANDLINE) {
			vector<string> args = split(commandline, ' ');
			fname = args.at(args.size() - 1);
			
			if (LoadFileToEditor(/*datadir + */ fname))
				SetTitle(
					(const char*)
					(apptitle + " - " + fname).c_str()
				);
		}
	}

	bool menu = true;
	float ifs =1;
	int width = rightSide;



	int currFont;
	void RenderGui(float deltaT) {
		SetNextWindowPos(ImVec2(0, 0));
		SetNextWindowSize(ImVec2(width, 0));
		Begin("Menu" , &menu, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar);
		Text("%s", (fname + " (" + lastaction +")").c_str());
		Button("New"); SameLine();
		if (Button("Save")) {
			SaveEditorToFile(fname);
		}SameLine();
		if (Button("Close")) {
			SaveEditorToFile(fname);
			fname = "";
		}

		if (Combo("StartFont", &currFont, availableFontsNullSeparatedForImGuiUse.c_str())) {
			SavePreference("Font", availableFonts[currFont]);
		}

#ifdef WIN32
		if (Button(":Hosts:")) LoadFileToEditor("C:\\Windows\\System32\\drivers\\etc\\hosts"); SameLine();
        if (Button(":TestXEdit:")) LoadFileToEditor("C:\\Users\\aliveb\\TestXEdit.txt");
#else
        if (Button(":Hosts:")) LoadFileToEditor("/etc/hosts");SameLine();
        if (Button(":TestXEdit:")) LoadFileToEditor("/Users/liveb/TestXEdit.txt");
#endif
        
		SliderFloat(": Zoom", &ifs, 1.0, 2.0);
		End();
		//SetNextWindowPos(ImVec2(0,80));
		ImGui::SetWindowFontScale(float(ifs));
		if (fname.size()>0) ted.Render(fname.c_str(), ImVec2(width-20, 0));
	}

	void UpdateCustom(GameObject* gob,int instanceNo, float deltaT) {
		if (gob->is(logo)) { //Apply Asset Specific Logic
		
		}
	}

};


