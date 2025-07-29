// Copyright (c) 2018 AcnodLabs Inc

#include "..\XText\TextEditor.hpp"


class /**/ App : public AlgeApp {
	GameObject bg;
	GameObject logo;
	TextEditor ted;
	string fname;
	
	string lastaction = "Ready";
	string defaultSystemFile = "SystemA.stx";

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

	bool Load(string AppTitle, string filename) {
		
		SetTitle(("Editing:"+AppTitle).c_str());
		return LoadFileToEditor(RemoveQuotesIfFound(filename));
	}

	bool LoadFileToEditor(string filename) {
		fname = filename;
		//	MessageBoxA(NULL, filename.c_str(), "LoadFileToEditor", MB_ICONASTERISK);
		vector<string> lines = ReadFileContents(filename);
		ted.SetTextLines(lines);
		lastaction = to_string(lines.size()) + " Lines Read";
		return (lines.size() > 0);
	}

public:

	virtual i2 getBackgroundSize() {
		i2 size = coord.getWorkstationSize();
		size.y *= 0.95f;//make some room for taskbars etc
		return size;
	}

	GameObject machines[8];
	GameObject modules[8];
	GameObject parts[8];
	string datadirpath;

	void Init(char* path) {
		aL10 draw;
		datadirpath = string(path);
		AlInit(STANDARD);
		AddDefaultCamera(Camera::CAM_MODE_2D, OrthoTypes::ORIGIN_IN_TOP_LEFT_OF_SCREEN);
		AddResource(&bg, "techno", "techno.jpg",XFunction_AutoScalingToFullScreen::AUTO_SCALING_FULLSCREEN);
		AddResource(&logo, "1x1", "acnode.jpg", 100)->SetPos(rightSide/2, logo.m_height/2);
		logo.JuiceType = JuiceTypes::JUICE_ROTY;

		Load(defaultSystemFile, defaultSystemFile);
		i2 machineXY;
		
		int xplacement = 200;
		int yplacement = 0;
		int xplacementMachines = xplacement;
		int xplacementModules = xplacement;
		int xplacementParts = xplacement;
		int yplacementMachines = bottomSide* 1/4;
		int yplacementModules = bottomSide* 2/4;
		int yplacementParts = bottomSide* 3/4;
		int xspacing = 20;
		short machineNo = -1;
		short moduleNo = 0;
		short partNo = 0;
		string lastSpec ="";
		for (auto spec : ted.GetTextLines()) {
			if (spec[0] == '-') {
				if (spec[0] == '-' && spec[1] != '-') { machineNo++; yplacement = yplacementMachines; 
					GameObject* m = AddResource(&machines[machineNo], "1x1", "a.jpg", 1);
					m->UUID = spec;
					m->SetPos(xplacementMachines, yplacement);
					xplacementMachines += (m->m_width + xspacing);
				}
				if (spec[0] == '-' && spec[1] == '-' && spec[2] != '-') { 
					GameObject* m = AddResource(&modules[moduleNo], "1x1", "aa.jpg", 1);
					m->UUID = spec;yplacement = yplacementModules;
					m->SetPos(xplacementModules, yplacement);
					xplacementModules += (m->m_width + xspacing);
					moduleNo++; 
				}
				if (spec[0] == '-' && spec[1] == '-' && spec[2] == '-') {
					GameObject* m = AddResource(&parts[partNo], "1x1", "aaa.jpg", 1);
					m->UUID = spec; yplacement = yplacementParts;
					m->SetPos(xplacementParts, yplacement);
					xplacementParts += (m->m_width + xspacing);
					partNo++; 
				}			
			}
			lastSpec = spec;
		}
		
	}
	
	//Query: count occurances of a character in a string (C++)
	gpt short countChar(const std::string& str, char target) {
		int count = 0;
		for (char ch : str) {
			if (ch == target) {
				count++;
			}
		}
		return count;
	}

	bool isChild(string lastSpec, string spec) {
		bool ret = false;
		if (countChar(spec,'-') == (countChar(lastSpec,'-') + 1)) {
			ret = true;
		};
		return ret;
	}

	void processInput(PEG::CMD* p, float deltaT) {
		if (p->command == CMD_KEYDOWN) {
			if (p->i1 == AL_KEY_DOT) {
#ifdef  WIN32
MessageBoxA(NULL, (char*)datadirpath.c_str(), "Data Dir", 0);
#endif //  WIN32

			output.pushP(CMD_SPAWN, (char*)datadirpath.c_str(), 0);
			}
		}
		if (p->command == CMD_TOUCH_START) {}
		if (p->command == CMD_SCREENSIZE) {}
		if (p->command == CMD_HHMM) {}
		if (p->command == CMD_COMMANDLINE) {
			vector<string> args = split(string((char*)p->param1), ' ');
			fname = args.at(args.size() - 1);
			defaultSystemFile = fname;
			Load( defaultSystemFile,defaultSystemFile);
			
		}
	}

	void RenderGui(float deltaT) {
		InfGuiHover();//Display Hover Information
		SetWindowFontScale(2.0);
		ted.Render("Markup");
	}

	void UpdateCustom(GameObject* gob,int instanceNo, float deltaT) {
		gob->ShowVitals();
	}

	void Deinit() {
		SaveEditorToFile(defaultSystemFile);
	}

};


