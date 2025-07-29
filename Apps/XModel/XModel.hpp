// Copyright c) 2018 AcnodLabs Inc

#include "TextEditor.hpp"
#undef pi
/*
#include "ImFileDialog.h"
#include "ImFileDialog.cpp"


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

reusable void replaceSubstring2(std::string& str, const std::string& oldStr, const std::string& newStr) {
	size_t pos = 0;
	while ((pos = str.find(oldStr, pos)) != std::string::npos) {
		str.replace(pos, oldStr.length(), newStr);
		pos += newStr.length();
	}
}

class /**/ App : public AlgeApp {
	GameObject bg;
	GameObject logo;
	GameObject prima;

	TextEditor alx;
	string datadir;

	
public:



	virtual i2 getBackgroundSize() {
		i2 size = coord.getWorkstationSize();
		size.y *= 0.95f;//make some room for taskbars etc
		return size;
	}

	vector<string> availableModels;
	vector<string> availableApps;

	string availableNullSeparatedForImGuiUse;
	string availableNullSeparatedAppNames;


	string getTag(string path) {
		vector<string> x = split(path, '/');
		string tag = split(x[x.size() - 1], '.')[0];
		
		return tag;
	}

	void appendTextToEditor(string n) {
		alx.SetText(alx.GetText() + "\n" + n);
	}

	short modelIdent;

	void LoadAndAdd(GameObject& obj, string path)
	{
		bool isObj = path.find_first_of(".obj") != string::npos;
		if (isObj) {
			AddResource(&obj, getTag(path)+ ".obj", ifs);
		}
		else {
			AddResource(&obj, getTag(path), ifs);
		}	
		CullFace = rm->models[prima.modelId]->is3D();
		usingWithCaution_decrementModelCounter();//Next Load will override current

		//AddResource(&obj, "techno", "techno.jpg", XFunction_AutoScalingToFullScreen::AUTO_SCALING_FULLSCREEN);
		lastaction = path;
		string l2 = replaceAll(path, ".tga", ".alx");
		LoadFileToEditor(l2);
	//	appendTextToEditor(obj.toString());
		
	}

	CAxis axis;


	void UpdateModels(string prefAlxName= "") {
		string prefAppNameWithPath = pathPrefix() + prefAppName + ".Assets/Data/";
		availableModels = GetModelNames(prefAppNameWithPath);
		if (prefAlxName.size() < 1) availableNullSeparatedForImGuiUse = "";
		int ic = 0;
		for (auto a : availableModels) {
			availableNullSeparatedForImGuiUse += a.substr(string(prefAppNameWithPath).size()) + '\0';
			if (a == prefAlxName) {
				currSel = ic;

			}
			ic++;
		}
		rm->Init((char*)prefAppNameWithPath.c_str());//point resourcemanger to the path
		

	}
	string prefAppName;

    bool CullFace = false;
    bool DrawAxis = true;
    
	void Init(char* path) {
		
		AlInit(STANDARD);
		AddDefaultCamera(Camera::CAM_MODE_2D, OrthoTypes::ORIGIN_IN_TOP_LEFT_OF_SCREEN);

		string names = "";
		
		availableApps = GetPeerAppNames(path);
		prefAppName = LoadPreference("AppName");

		int ic = currSelAppName;
		for (auto a : availableApps) {
			availableNullSeparatedAppNames += (a + '\0');
			if (prefAppName == a) {
				currSelAppName = ic;
			}
			ic++;
		}

		string prefAlxName = LoadPreference("AlxName");
		
		UpdateModels(prefAlxName);
		
		if (prefAlxName.size()> 0) {
			replaceSubstring(prefAlxName, ".tga", ".alx");
			LoadAndAdd(prima, prefAlxName);			
		}

		datadir = string(path);
		

		//AddResource(&bg, "techno", "techno.jpg",XFunction_AutoScalingToFullScreen::AUTO_SCALING_FULLSCREEN);
		
		logo.JuiceType = JuiceTypes::JUICE_PULSATE;
		SetTitle(apptitle.c_str());

	}

	string apptitle = "XModel by Acnodelabs (c) 2024";
	string lastaction = "Ready";
	int alxtoolvar_width = 64;
	int alxtoolvar_height = 64;

	string makeAlxSrc(int width, int height) {
		string atemplate = "//786//\n";
		atemplate += "obj[widthxheight]{\n";
				atemplate += "v[ -width.000000, -height.000000, 0.000000]\n";
				atemplate += "n[ 0.000000, 0.000000, 1.000000]\n";
				atemplate += "t[ 0.000000, 1.000000]\n";
				atemplate += "v[ width.000000, -height.000000, 0.000000]\n";
				atemplate += "n[ 0.000000, 0.000000, 1.000000]\n";
				atemplate += "t[ 1.000000, 1.000000]\n";
				atemplate += "v[ width.000000, height.000000, 0.000000]\n";
				atemplate += "n[ 0.000000, 0.000000, 1.000000]\n";
				atemplate += "t[ 1.000000, 0.000000]\n";
				atemplate += "v[ -width.000000, -height.000000, 0.000000]\n";
				atemplate += "n[ 0.000000, 0.000000, 1.000000]\n";
				atemplate += "t[ 0.000000, 1.000000]\n";
				atemplate += "v[ width.000000, height.000000, 0.000000]\n";
				atemplate += "n[ 0.000000, 0.000000, 1.000000]\n";
				atemplate += "t[ 1.000000, 0.000000]\n";
				atemplate += "v[ -width.000000, height.000000, 0.000000]\n";
				atemplate += "n[ 0.000000, 0.000000, 1.000000]\n";
				atemplate += "t[ 0.000000, 0.000000]\n";
				atemplate += "}\n";
				atemplate += "vertex_count[6]\n";
				atemplate += "//Good Luck from XModel !!\n";
				string btemplate = replaceAll(atemplate, "width", to_string(width));
				string ctemplate = replaceAll(btemplate, "height", to_string(height));
		return ctemplate;
	}

	bool dontsave = false;

	bool SaveEditorToFile(string filename) {
		if (dontsave) return false;
		fname = filename;
		if (filename.size() < 1) {
			lastaction = "Nothing Saved";
			return false;
		}
		//	MessageBoxA(NULL, filename.c_str(), "LoadFileToEditor", MB_ICONASTERISK);
		vector<string> lines = alx.GetTextLines(); 
		WriteFileContents(filename, lines);
		lastaction = to_string(lines.size()) + " Lines Saved";
		return (lines.size() > 0);
	}

	bool LoadFileToEditor(string filename) {
		fname = filename;
	//	MessageBoxA(NULL, filename.c_str(), "LoadFileToEditor", MB_ICONASTERISK);
		vector<string> lines = ReadFileContents(filename);
		dontsave = lines.size() > 50;
		if (dontsave) {
			alx.SetText("Won't edit lines > 50");
			return false;
		};
		alx.SetTextLines(lines);
	//	lastaction = to_string(lines.size()) + " Lines Read";
		return (lines.size() > 0);
	}

	int nArgs =0;

	string fname;
	void processInput(PEG::CMD* p, float deltaT) {
		if (p->command == CMD_KEYDOWN) {
		
			if (p->i1 == 'W' || p->i1 == 'w') {
				wireframe = !wireframe;
			}
		
		}

		if (p->command == CMD_MOUSEWHEEL) {
			int d = p->i1;
		//	int zDelta = p->i2;
			ifs += d / 200.;
		}

		if (p->command == CMD_TOUCH_START) {}
		if (p->command == CMD_SCREENSIZE) {
			width = p->i1;
		}
		if (p->command == CMD_HHMM) {}
		if (p->command == CMD_COMMANDLINE) {
			vector<string> args = split(commandline, ' ');
			fname = args.at(args.size() - 1);
			
			//if (LoadFileToEditor(/*datadir + */ fname))
			//	SetTitle(
			//		(const char*)
			//		(apptitle + " - " + fname).c_str()
			//	);
		}
	}

	bool menu = true;
	float ifs =1;
	int width = rightSide;


	int currSel;
	int currSelAppName;
	f3 rotation;

	void RenderGui(float deltaT) {
	//	SetNextWindowPos(ImVec2(rightSide/2, bottomSide/2));
		alx.Render("ALX", ImVec2(rightSide / 4, bottomSide / 4), true);

		SetNextWindowPos(ImVec2(0, 0));
		SetNextWindowSize(ImVec2(width, 0));
		Begin("Menu", &menu);// , ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar);
		Text("%s", (fname + " (" + lastaction +")").c_str());
		Button("New"); SameLine();

		if (Button("Close")) {
			SaveEditorToFile(fname);
			fname = "";
		}

		if (Combo("AppName", &currSelAppName, availableNullSeparatedAppNames.c_str())) {
			prefAppName = availableApps[currSelAppName];
			SavePreference("AppName", prefAppName);
			availableNullSeparatedForImGuiUse = "";
			UpdateModels();
		}
		

        

		if (SliderFloat(": Zoom", &ifs, 1.0, 50.0)) {
			prima.scale = ifs;
		}

		//JuiceControls

		//Rot Controls
		bool rotated = false;
		if (SliderFloat("RotX (Red X Axis)", &rotation.x, -180.0, 180.0)) rotated = true; 
		SameLine(); 
		if (Button("Reset Rot")) {
			rotation.clear(); rotated = true;
		}
		if (SliderFloat("RotY (Grn Y Axis)", &rotation.y, -180.0, 180.0)) rotated = true;
		if (SliderFloat("RotZ (Blu Z Axis)", &rotation.z, -180.0, 180.0)) rotated = true;
		int w = prima.getOwnRect().Right - prima.getOwnRect().Left;
        if (prima.modelId>=0)
            Text(rm->models[prima.modelId]->is3D() ? "3D" : "2D"); SameLine();

		Text(("Width:" + to_string(w)).c_str());SameLine();
		Text(("Bounds:" + prima.getOwnRect().toStr()).c_str()); SameLine();
        Checkbox("Wireframe",&wireframe);SameLine();

        Checkbox("Axis",&DrawAxis); SameLine();
		Text("CullFace", &CullFace);


		if (rotated) prima.SetRot(rotation);

		End();
		//SetNextWindowPos(ImVec2(0,80));
		//ImGui::SetWindowFontScale(float(ifs));

		Begin("Alx Tools");
		ImGui::InputInt("Width", &alxtoolvar_width);
		ImGui::InputInt("Height", &alxtoolvar_height);
		if (ImGui::Button(
				(
				string("Gen") + to_string(alxtoolvar_width) + "x" + to_string(alxtoolvar_height)
				).c_str()
		
			)) {
			alx.SetText(makeAlxSrc(alxtoolvar_width, alxtoolvar_height));
		}SameLine();
		if (Button("Save")) {
			SaveEditorToFile(fname);
		}
		End();


		Begin("Juice");
		static int e = 0;

		ImGui::InputFloat("JuiceSpeed",& prima.JuiceSpeed);
		ImGui::InputFloat("JuiceDuration", &prima.JuiceDuration);
		ImGui::InputDouble("juice_sin_angle", &prima.juice_sine_angle);

		ImGui::RadioButton("NIL", &e, 0);
		for (int i = 1; i < JuiceTypes::JUICES_END; i++)
			ImGui::RadioButton((JuiceName(i)).c_str(), &e, i);
		prima.JuiceType = e;
		End();
		if (!rotated) rotation = prima.GetRot();
		if (e == 0) {
			prima.JuiceReset();
		}

		Begin("Models");
		int imsel = 0;
		int im = 0;
		for (auto m : availableModels) {

			string name = availableModels[im];
			replaceSubstring(name, pathPrefix(), "");

			if (RadioButton(name.c_str(), &currSel, im)) {
			//	currSel = imsel;
			//	if (Combo("StartAlx", &currSel, availableNullSeparatedForImGuiUse.c_str())) 
				{
					string prefAlxName = availableModels[currSel];
					SavePreference("AlxName", prefAlxName);
					ifs = 1.0;
					LoadAndAdd(prima, prefAlxName);
				}
				break;
			}
			im++;
		}


		End();

		//if (lastaction.size()>0)
			
	}

	void UpdateCustom(GameObject* gob,int instanceNo, float deltaT) {
		if (gob->is(prima)) { //Apply Asset Specific Logic
         //   if (CullFace) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
			if (DrawAxis) axis.glDraw();
			int w1 = prima.getOwnRect().Bottom - prima.getOwnRect().Top;
			int w2 = prima.getOwnRect().Right - prima.getOwnRect().Left;
			int w = min(w1,w2);
			
			if (w * ifs < 10) ifs += 0.02;//
			prima.scale = ifs;
		}
	}

};


