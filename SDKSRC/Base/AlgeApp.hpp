
#include <string>
#include <iostream>
#include <filesystem>

#define GLES_SILENCE_DEPRECATION
#define FIRST 1
#define SCL (2400/2400)
#define P2S 100*SCL
#define S2P 0.01*SCL
#define gpt

//for ease only
#define Definition extern class 

#include "Ada/AlgeSDK.ads.h"
#include "camera.h"

#ifndef NO_BOX2D
#include <Box2D/Box2D.h>
#endif
#ifndef NO_NATS
#include "../../Tools/CNetMsg/CNetMsg.h"
extern CNetMsg netmsg;
#endif


#include "dimensions.h"
#include "drawtext.h"
#include <sstream>
#include <iomanip>
#include <regex>
#include <algorithm> // for transform(std::string)
#include "XFunctions.hpp"
#include "icetop.cpp"
#include <fstream>
/// <summary>
/// REUSABLE FUNCTIONS
/// </summary>
/// <param name="input"></param>
/// <returns></returns>
std::string RemoveQuotesIfFound(const std::string& input) {
	std::string result;

	for (char c : input) {
		if (c != '"') {
			result += c;
		}
	}

	return result;
}

void WriteFileContents(string filename, vector<string> lines) {
	ofstream myfile;
	myfile.open(RemoveQuotesIfFound(filename));
	int n = 0;
	for (auto line : lines) {
		myfile << line + "\n";
		n++;
	}
	myfile.close();
}
vector<string> ReadFileContents(string filenam) {
	vector<string> ret;
	FILE* f = NULL;
	string filename = RemoveQuotesIfFound(filenam);
#ifdef fopen_s
    fopen_s(&f, filename.c_str(), "rb");
#else 
    f = fopen(filename.c_str(), "rb");
#endif
	if (f) {
		//		MessageBoxA(NULL, filename.c_str(), "FileHasOpened", MB_ICONEXCLAMATION);
		fclose(f);
	}
	if (f == NULL) {
	//	MessageBoxA(NULL, filename.c_str(), "File could not be opened", MB_ICONEXCLAMATION);
	}
	std::ifstream file(filename.c_str());
	std::string str;

	while (std::getline(file, str))
	{
		//MessageBoxA(NULL, str.c_str(), "ReadingFile", MB_ICONEXCLAMATION);
		ret.push_back(str);
	}
	file.close();
	return ret;
}
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NO_BOX2D
class DbgDraw : public b2Draw
  {
  
  public:
    
	  i2 Map(const b2Vec2 point) {
		  return i2(P2S * point.x , P2S *point.y);
	  }

    int m_rightSide = 0, m_bottomSide = 0;
    
      void Set(int rightSide, int bottomSide) {
          m_rightSide = rightSide;
          m_bottomSide = bottomSide;
      }
      
    void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
        //set up vertex array
            GLfloat glverts[16]; //allow for polygons up to 8 vertices
            glVertexPointer(2, GL_FLOAT, 0, glverts); //tell OpenGL where to find vertices
            glEnableClientState(GL_VERTEX_ARRAY); //use vertices in subsequent calls to glDrawArrays
            
            //fill in vertex positions as directed by Box2D
            for (int i = 0; i < vertexCount; i++) {
				i2 vv = Map(vertices[i]);
				glverts[i * 2] = vv.x;
				glverts[i * 2 + 1] = vv.y;
            }
            
            //draw solid area
            glColor4f( color.r, color.g, color.b, 1);
            glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
          
            //draw lines
            glLineWidth(3); //fat lines
            glColor4f( 1, 0, 1, 1 ); //purple
            glDrawArrays(GL_LINE_LOOP, 0, vertexCount);
    }
    void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
        //set up vertex array
            GLfloat glverts[16]; //allow for polygons up to 8 vertices
            glVertexPointer(2, GL_FLOAT, 0, glverts); //tell OpenGL where to find vertices
            glEnableClientState(GL_VERTEX_ARRAY); //use vertices in subsequent calls to glDrawArrays
            
            //fill in vertex positions as directed by Box2D
            for (int i = 0; i < vertexCount; i++) {
			  i2 vv = Map(vertices[i]);
              glverts[i*2]   = vv.x;
              glverts[i*2+1] = vv.y;
            }
            
            //draw solid area
            glColor4f( color.r, color.g, color.b, 1);
            glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
          
            //draw lines
            glLineWidth(3); //fat lines
            glColor4f( 1, 0, 1, 1 ); //purple
            glDrawArrays(GL_LINE_LOOP, 0, vertexCount);
    }

	aL10 al;

	//
	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
	{
		const float32 k_segments = 16.0f;
	//(char*)	const float32 k_increment = 2.0f * b2_pi / k_segments;
	//	float32 sinInc = sinf(k_increment);
	//	float32 cosInc = cosf(k_increment);
		b2Vec2 r1(1.0f, 0.0f);
	//	b2Vec2 v1 = center + P2S*radius * r1;
		al.Circle(P2S * radius, k_segments, Map(center));
	//	for (int32 i = 0; i < k_segments; ++i)
	//	{
	//		// Perform rotation to avoid additional trigonometry.
	//		b2Vec2 r2;
	//		r2.x = cosInc * r1.x - sinInc * r1.y;
	//		r2.y = sinInc * r1.x + cosInc * r1.y;
	//		b2Vec2 v2 = center + radius * r2;
	////		m_lines->Vertex(v1, color);
	////		m_lines->Vertex(v2, color);
	//		r1 = r2;
	//		v1 = v2;
	//	}
	}

	//
	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
	{
		const float32 k_segments = 16.0f;
		const float32 k_increment = 2.0f * b2_pi / k_segments;
		float32 sinInc = sinf(k_increment);
		float32 cosInc = cosf(k_increment);
	//	b2Vec2 v0 = center;
		b2Vec2 r1(cosInc, sinInc);
		b2Vec2 v1 = center + radius * r1;
		b2Color fillColor(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
		
		glColor4f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
		for (int radiu = 2; radiu <= P2S *(radius); radiu += 2)
			al.Circle(P2S *radius, k_segments, Map(center));
		glColor4f(.5, .5,.5,.5);
		return;

		for (int32 i = 0; i < k_segments; ++i)
		{
			// Perform rotation to avoid additional trigonometry.
			b2Vec2 r2;
			r2.x = cosInc * r1.x - sinInc * r1.y;
			r2.y = sinInc * r1.x + cosInc * r1.y;
			b2Vec2 v2 = center + radius * r2;
	//		m_triangles->Vertex(v0, fillColor);
	//		m_triangles->Vertex(v1, fillColor);
	//		m_triangles->Vertex(v2, fillColor);
			r1 = r2;
			v1 = v2;
		}

		r1.Set(1.0f, 0.0f);
		v1 = center + radius * r1;
		for (int32 i = 0; i < k_segments; ++i)
		{
			b2Vec2 r2;
			r2.x = cosInc * r1.x - sinInc * r1.y;
			r2.y = sinInc * r1.x + cosInc * r1.y;
			b2Vec2 v2 = center + radius * r2;
		//	m_lines->Vertex(v1, color);
		//	m_lines->Vertex(v2, color);
			r1 = r2;
			v1 = v2;
		}

		// Draw a line fixed in the circle to animate rotation.
		b2Vec2 p = center + radius * axis;
	//	m_lines->Vertex(center, color);
	//	m_lines->Vertex(p, color);
	}

	//
	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
	{
		glColor4f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
		i2 ip1 = Map(p1);
		i2 ip2 = Map(p2);
		al.Line(ip1.x, ip1.y, ip2.x, ip2.y);
		glColor4f(.5, .5, .5, .5);

		//m_lines->Vertex(p1, color);
		//m_lines->Vertex(p2, color);
	}

	//
	void DrawTransform(const b2Transform& xf)
	{
		const float32 k_axisScale = 0.4f;
		b2Color red(1.0f, 0.0f, 0.0f);
		b2Color green(0.0f, 1.0f, 0.0f);
		b2Vec2 p1 = xf.p, p2;
		i2 ip1 = Map(p1);
		
	//	m_lines->Vertex(p1, red);
		p2 = p1 + k_axisScale * xf.q.GetXAxis();
		i2 ip2 = Map(p2);
		//	m_lines->Vertex(p2, red);
		glColor4f(1., 0., 0.,0.5);
		al.Line(ip1.x, ip1.y, ip2.x, ip2.y);

	//	m_lines->Vertex(p1, green);
		p2 = p1 + k_axisScale * xf.q.GetYAxis();
		i2 ip22 = Map(p2);
	//	m_lines->Vertex(p2, green);
		glColor4f(1., 0., 0., 0.5);
		al.Line(ip1.x, ip1.y, ip22.x, ip22.y);
	}

	//
	void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
	{
		DrawCircle(p, size, color);
//		m_points->Vertex(p, color, size);
	}
  };
#endif
#ifndef NO_IMGUI
using namespace ImGui;

class ImKeyboard {
    
public:
    PEG* peg = NULL;
    bool Visible = false;
    char z = 0;
    char row1[20] = {'1',z,'2',z,'3',z,'4',z,'5',z,'6',z,'7',z,'8',z,'9',z,'0',z};
    char row2[20] = {'Q',z,'W',z,'E',z,'R',z,'T',z,'Y',z,'U',z,'I',z,'O',z,'P',z};
    char row3[18] =  {'A',z,'S',z,'D',z,'F',z,'G',z,'H',z,'J',z,'K',z,'L',z};
    char row4[14] = {'Z',z,'X',z,'C',z,'V',z,'B',z,'N',z,'M',z};
    char row5[6] = {',',z,' ',z,'.',z};
    
    ImKeyboard() {
        
    }
   

    void onKey(char key) {
        if (peg) peg->pushI(CMD_KEYDOWN, key, key);
    }
    
	bool CenteredButton(char* text) {
		float width = ImGui::GetWindowSize().x;
		float btnWidth = CalcTextSize(text).x;
		float centre_position_for_button = (width - btnWidth) / 2;
		ImGui::SetCursorPosX(centre_position_for_button);
		return Button(text);
	}
	float getCursorPosFor(int i, char * txt,int max) {
		int mid = max / 2;
		int pad = 6;
		float totwidth = ImGui::GetWindowSize().x;
		float btnWidth = CalcTextSize(txt).x;
		float numelempossible = (totwidth)/ (btnWidth+pad);
		float ret = 0.0;
		for (int j = 0; j < int(numelempossible); j++) {
			float posx = (btnWidth + pad) * j;
			if (j == i) {
				ret = posx;
				break;
			}
		}
		return ret;
	}

    void Render() {

        if (!Visible) {
			Visible = CenteredButton((char*) "<<  KEYBOARD  >>");
            return;
        };
        if (CenteredButton((char*)"<<    HIDE    >>")) {
            Visible=false;          
        };
      //  static ImVec2 refButtonSize;
		ImGui::ShowStyleSelector("Style Selector");
		auto pDrawList = ImGui::GetWindowDrawList();

		pDrawList->AddRect(ImVec2(10, 10), ImVec2(100, 100), ImColor(255, 0, 0));
		pDrawList->AddText(ImVec2(10, 10), ImColor(255, 0, 0), "test");


        for (int i=0; i<20; i+=2) {	
	//		ImGui::SetCursorPosX(getCursorPosFor(i,&row1[i], 20));
			if (ImGui::Button(row1+i)) {onKey(row1[i]);};
            if (i<18) ImGui::SameLine();
        }
        for (int i=0; i<20; i+=2) {
	//		ImGui::SetCursorPosX(getCursorPosFor(i, &row2[i], 20));
            if (ImGui::Button(row2+i)) {onKey(row2[i]);};
            if (i<18) ImGui::SameLine();
        }
        for (int i=0; i<18; i+=2) {
            if (ImGui::Button(row3+i)) {onKey(row3[i]);};
            if (i<16) ImGui::SameLine();
        }
        for (int i=0; i<14; i+=2) {
            if (ImGui::Button(row4+i)) {onKey(row4[i]);};
            if (i<12) ImGui::SameLine();
        }
        for (int i=0; i<6; i+=2) {
            if (i==2) {
                if (ImGui::Button(row5+i,ImVec2(100,20))) {onKey(row5[i]);};
            } else {
                if (ImGui::Button(row5+i)) {onKey(row5[i]);};
            }
            if (i<4) ImGui::SameLine();
        }
        ImGui::SameLine();
        
    }
};

class ImHelper {
	
public:

	class AlgeApp* m_app;

	ImFont *preloaded, *font_Small, *font_Medium, *font_Tiny, *font_Huge, *font_Large, * font_Enormous;

	ImFont* fontarray[64];

	ImHelper() {

	}
	float m_scale;
	string m_path;

	/*
imgui.WINDOW_NO_TITLE_BAR
imgui.WINDOW_NO_RESIZE
imgui.WINDOW_NO_MOVE
imgui.WINDOW_NO_SCROLLBAR
imgui.WINDOW_NO_SCROLL_WITH_MOUSE
imgui.WINDOW_NO_COLLAPSE
imgui.WINDOW_ALWAYS_AUTO_RESIZE
imgui.WINDOW_NO_SAVED_SETTINGS
imgui.WINDOW_NO_INPUTS
imgui.WINDOW_MENU_BAR
imgui.WINDOW_HORIZONTAL_SCROLLING_BAR
imgui.WINDOW_NO_FOCUS_ON_APPEARING
imgui.WINDOW_NO_BRING_TO_FRONT_ON_FOCUS
imgui.WINDOW_ALWAYS_VERTICAL_SCROLLBAR
imgui.WINDOW_ALWAYS_HORIZONTAL_SCROLLBAR
imgui.WINDOW_ALWAYS_USE_WINDOW_PADDING
     */
	string Pad(string Text, bool underline = false) {
		//"Reset Hr">>"\n Reset Hr \n__________\n";
		int l = (int)Text.length();//l = actual length
		std::string s(l+2, underline?'_':' ');
		string nl("\n");
		string sp(" ");
		Text = nl + sp +Text+ sp +nl+ s+ nl;
		return Text;
	}

	bool CenterBtn(std::string text, bool underline) {
		string pText = Pad(text, underline);
		//float font_size = ImGui::GetFontSize() * (text.size()+2) / 2;
		//ImGui::SameLine(ImGui::GetWindowSize().x / 2 -    font_size + (font_size / 2) );
		bool b = ImGui::Button(pText.c_str());
	//	Dummy(ImVec2(0,1));
        return b;
	}
	static bool isHovered;
	static bool isFocused;

	void ImTest1() {
		// Using the font texture as an example, you'd obviously use your emulator's screen texture instead
		ImGuiIO& io = ImGui::GetIO();
		ImTextureID textureId = io.Fonts->TexID;
		ImVec2 textureSize = ImVec2(200, 200);

		ImGui::Begin("GH-4233 Exam");

		ImGui::ImageButton(textureId, textureSize);
		isHovered = ImGui::IsItemHovered();
		isFocused = ImGui::IsItemFocused();

		ImVec2 mousePositionAbsolute = ImGui::GetMousePos();
		ImVec2 screenPositionAbsolute = ImGui::GetItemRectMin();
		ImVec2 mousePositionRelative = ImVec2(mousePositionAbsolute.x - screenPositionAbsolute.x, mousePositionAbsolute.y - screenPositionAbsolute.y);
		ImGui::Text("Is mouse over screen? %s", isHovered ? "Yes" : "No");
		ImGui::Text("Is screen focused? %s", isFocused ? "Yes" : "No");
		ImGui::Text("Position: %f, %f", mousePositionRelative.x, mousePositionRelative.y);
		ImGui::Text("Mouse clicked: %s", ImGui::IsMouseDown(ImGuiMouseButton_Left) ? "Yes" : "No");

		ImGui::End();
	}

	ImFont* getImFont(int iFontSize) {
		if (iFontSize > 63) iFontSize = 63;
		return fontarray[iFontSize];
	}

	void SetFont(char* path, const char* fontfile, int siz = 55) {
		float platformAdjustmentVisuallyNoted = 1.0; //ref device galaxy note 10
#ifdef WIN32
		platformAdjustmentVisuallyNoted = 0.847;
#endif
#ifdef MACOSX
        platformAdjustmentVisuallyNoted = 0.326;
#endif
		imguiFont.Use(string(path) + (path[0]==0?"":"/") + string(fontfile), siz * platformAdjustmentVisuallyNoted);
	}

	//e.g Call with 'Roboto-Bold.ttf'
	void Init(char* path, string ttf, i2 ScreenSize,int screenwidth = 1500.0) {
		int scale = (int)((1.0 / 40.0) * ScreenSize.x);
		ImGuiIO& io = ImGui::GetIO();
		m_scale = scale;
		m_path = string(path);
        string fttf = string(m_path);
#ifdef ALGE_WINDOWS
        fttf = fttf + ttf;
#else
        fttf = fttf + "/" + ttf;
#endif

#ifdef ANDROID
		ImGui::GetStyle().TouchExtraPadding = ImVec2(4.0F, 4.0F);
#endif
#ifdef IOS
		ImGui::GetStyle().TouchExtraPadding = ImVec2(4.0F, 4.0F);
#endif
		
		FILE* f = NULL;
#ifdef fopen_s
		fopen_s(&f, fttf.c_str(), "r");
#else
        f = fopen(fttf.c_str(), "r");
#endif
		if (f) {
			fclose(f);
		}
		else {
//			MessageBoxA(NULL, fttf.c_str(), "Font File Missing", MB_ICONEXCLAMATION);
			return;
		}

		float n = 5;

		if (ttf.length()) {

			fontarray[0] = io.Fonts->AddFontDefault();
			for (int i = 1; i < 64; i++) 
				fontarray[i] = io.Fonts->AddFontFromFileTTF(fttf.c_str(), i);

			font_Tiny = fontarray[18-1];// io.Fonts->AddFontFromFileTTF(fttf.c_str(), 4 * n);
			font_Small = fontarray[22-1];//io.Fonts->AddFontFromFileTTF(fttf.c_str(), 8 * n);//Imgui will use the top file in init
			font_Medium = fontarray[28-1];//io.Fonts->AddFontFromFileTTF(fttf.c_str(), 16 * n);
			font_Large = fontarray[42-1];//io.Fonts->AddFontFromFileTTF(fttf.c_str(), 32 * n);
			font_Huge = fontarray[58-1];//io.Fonts->AddFontFromFileTTF(fttf.c_str(), 64 * n);
			font_Enormous = fontarray[64-1];//io.Fonts->AddFontFromFileTTF(fttf.c_str(), 128 * n);
			preloaded = font_Large;
		}
		else {
			ImFont* f = io.Fonts->AddFontDefault();
			font_Tiny = f;
			font_Small = f;
			font_Medium = f;
			font_Large = f;
			font_Huge = f;
			font_Enormous = f;
			preloaded = f;
		}

	}

	int slidervalue;
	ImVec2 mp;
	bool essentials_active = true;

	void ShowEssentials(GameObject* g, float dt) {
		// Create a window called "My First Tool", with a menu bar.
		mp.x = g->GetPos().x;
		mp.y = g->GetPos().y;
		SetNextWindowPos(mp);
		PushFont(font_Small);
		Begin(g->UUID.c_str(), &essentials_active, false);
		TextColored(ImVec4(1,1,0,1), "@%.0f,%.0f", mp.x,mp.y);
		End();
		PopFont();
	}
	bool glo = true;

	void ShowTextLine(i2 pos, string text, f3 color, ImFont* hFont) {
		SetNextWindowPos(ImVec2(pos.x,pos.y));
		PushFont(hFont);
		int flags =ImGuiWindowFlags_NoBackground|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings|ImGuiWindowFlags_NoInputs;
		flags = flags | ImGuiWindowFlags_NoBackground;
		Begin(text.c_str(), &glo, flags);
		TextColored(ImVec4(color.x,color.y,color.z,1), "%s", text.c_str());
		End();
		PopFont();
	}

	int rb1;
	void ShowRadioButton(i2 pos, string text, f3 color, ImFont* hFont) {
		SetNextWindowPos(ImVec2(pos.x,pos.y));
		PushFont(hFont);
		int flags =ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings|ImGuiWindowFlags_NoInputs;
		flags = flags | ImGuiWindowFlags_NoBackground;
		//flags = 0;
		Begin(text.c_str(), &glo, flags);
		RadioButton(text.c_str(),&rb1);
		End();
		PopFont();
	}

	void ShowTextLine(i2 pos, string text) {
		f3 color(0.5,0.5,0.5);
		ImFont* hFont = font_Medium;
		ShowTextLine(pos,text,color,hFont);
	}


	void ImScaleText(float percentage_of_screen) {
		ImGui::SetWindowFontScale((ImGui::GetIO().DisplaySize.x/10000.)*percentage_of_screen);
	}

	i2 leftCorner(i2 wh) {
		return i2(wh.x*0.015,wh.y*0.05);
	}

	int fps =0;
	void ShowFPS(i2 screen_dimensions, string msg, float deltaT, f3 color) {
		fps = int(1.0/ deltaT);
		ShowTextLine(leftCorner(screen_dimensions), (msg + to_string(fps)), color, font_Small);
	}

};
#endif

//The Coordinate problem
class Coord {
	i2 resolutionReported;
	f2 dev_emu_size;
public:
	enum { 
		ORIENTATION_PORTRAIT,
		ORIENTATION_LANDSCAPE
	};

	float scaleReported;
	i2 phres;
	i2 sdkres;
	i2 guires;
	

	void somethingChaged() {
		phres = resolutionReported;
		sdkres = resolutionReported;
		guires = resolutionReported;
	}

	short orientation;
	string LastError;
	Coord() {
		//dev_emu_size = f2(., .);
		LastError = "";
		orientation = ORIENTATION_PORTRAIT;
	}


	void setOrientation(short ORIENTATION) {
		orientation = ORIENTATION;
	}

	float getGuiScale() {
		float ret = 1.0;
#ifdef ALGE_WINDOWS
		if (orientation==ORIENTATION_PORTRAIT)
			ret = dev_emu_size.y / resolutionReported.x;
		else 
			ret = dev_emu_size.x / resolutionReported.x;
		ret = ret / 2;//TODO What the HECK 
#endif // ALGE_WINDOWS

		return ret;
	}

	i2 workstationsize;

	void setWorkstationSize(i2 xy) {
		workstationsize.x = xy.x;
		workstationsize.y = xy.y;
	}

	i2 getWorkstationSize() { 
		return workstationsize;
	}
	
	bool once = true;
	bool setResolutionReported(i2 xy, float retinaScale = 1.0) {
		if (retinaScale <= 0) return false;
		//check if app requests window size;
		
		scaleReported = retinaScale;

		this->resolutionReported.x = xy.x;
		this->resolutionReported.y = xy.y;
		
		if (once) {
#ifdef ALGE_WINDOWS_DEPRECATED_BLOCK
			dev_emu_size = f2(2400.,1080.);//half half is to accomodate it on a 2K montor
			bool usefulldesktop = true;		
			if (usefulldesktop) {
				///////////OVERRIDE TO USE FULL
				dev_emu_size.x = xy.x;
				dev_emu_size.y = xy.y;
				if (isPortrait()) dev_emu_size = dev_emu_size.flip();
			} else {
				float ar = dev_emu_size.x / dev_emu_size.y;
				dev_emu_size.y = 0.95 * xy.y;
				dev_emu_size.x = 0.95 * dev_emu_size.y / ar;
				if (!isPortrait()) dev_emu_size = dev_emu_size.flip();
			}
#else 
			dev_emu_size.x = xy.x;
			dev_emu_size.y = xy.y;
		
#endif
			
		//	once = false;

		}



		this->sdkres.x = dev_emu_size.x / scaleReported;// e.g 1 for normal 2 for retina
		this->sdkres.y = dev_emu_size.y / scaleReported;

		this->guires.x = getResolutionForGameEngine().x;
		this->guires.y = getResolutionForGameEngine().y;

		this->phres.x = S2P * guires.x;//One unit of ph is 100 pixels, 9.8m/s in physics world should translate to 98 pixels per second
		this->phres.y = S2P * guires.y;
		
		return true;
	}

	float getScaleOfOjects(GameObject* refObject) {
		if (!refObject) return 1.0;
		float w = float(refObject->getTexWidth());
	    float so = resolutionReported.x / w;//* getResolutionForGameEngine().x;
        return so;
	}


	void ImGuiMain(int x, int y, float guiScale) {

	}

	float getImGuiFontScale() {
		float scale = dev_emu_size.y / resolutionReported.y;
		return scale;
	}

	i2 getResolutionForPhysicsEngine() {return phres;}
	
	i2 getResolutionForGameEngine() {
		if (orientation == Coord::ORIENTATION_PORTRAIT)
			return sdkres;
		else
			return sdkres.flip();
	}

	virtual i2 getResolutionForGuiUsage() {
		if (orientation == Coord::ORIENTATION_PORTRAIT)
			return guires;
		else
			return guires.flip();
	}

	void Test() {
		return;
		i2 ossays(1920, 1200);
		Coord c;
	//	c.setResolutionReported(this, ossays, 2.0);

	}

};



class NewTech {
public:
	/*https://developer.android.com/about/versions/14        3 Dec 23*/
	/**/
	/**/

	class Feature {
		class FeatureResult {
		public:
			
		};
	public:
		FeatureResult Setup() {};
		FeatureResult Start() {};
		FeatureResult Stop() {};
	};

	class AlgeFeature : public Feature {
	};
	class GameLoop : public AlgeFeature {

	};

	class AndroidFeature : public Feature {
	};
	class Android14Feature : public AndroidFeature {};
	class IOSFeature : public Feature {};

	//Android14
	class GrammaticalInflectionAPI : public Android14Feature {};
	class ScreenShotDetection : public Android14Feature {};
	class PredictiveBackInAppAnimations : public Android14Feature {};
	class AppMetaDataBundles : public Android14Feature {};
	class LargeScreenOEMperAppOverrides : public Android14Feature {};
	class CredentialsManager : public Android14Feature {};

};






extern void appSize(int, int, int);

class AlgeApp
#ifndef NO_BOX2D
	: public b2ContactListener
#endif
{
public:
	Coord coord; //Primary Coordinate Provider
	bool desktop;
	string wall_msg;
	string name;
 #ifndef NO_BOX2D
	b2World* pWorld;
	b2World* world;
#endif
	CFTFont text;
	FILE* f = NULL;
	vector<PosRotScale*> touched_bodies;

	CTrackBall trackball;
	//CEasyBullet bullet;
	short renderSchemeVersion;//1=> Render processInput 2->UpdateCustom GOB's
	CAnimator can;
	CTimed animStepTimed;
	FontMap10x10 fontsV1;
	int keyframe, nseq, screenX, screenY;

	f3 eye, eyerot, tgt, up;
	float timeVar, animstep, deltaT;
	float bz, width, height;
	GameObject* gobs[1024]; short nGobs;
	GameObject* selectedObject;
    float screenScale; //for retina it is 2 otherwise 1
   
	CAxis xyz;
#ifndef  NO_IMGUI
	ImHelper imHelper;
#endif
	short currentscene;

	int iUnassigned;
	int dcursor;
	int screen_y;
	PEG input, output;
	static CResourceManager m_rm;
	static CResourceManager* rm;
	aL10 AL10;
	int backgroundModelId = 0;//used for dimming

	bool inhibitRender = false;
	bool edit;
	bool wireframe = false;
	float timeMultiplier = 1.0;

	Camera aCamera;
	
	GameObject origin;

#ifndef NO_FONTLIB
	struct dtx_font * font;
#endif

	AlgeApp() :
		iUnassigned(-1),
		timeVar(0.0) {
		edit = false;
		renderSchemeVersion = 2;
		width = 0;
		height = 0;
		dcursor = 0;
		screen_y = 0;
		keyframe = 1;
		animstep = 0.0;
		bz = 1.0;
		nseq = 0;
		screenX = iUnassigned;
		screenY = iUnassigned;
		selectedObject = nullptr;
		edit = false;
		iSelectedObject = 0;
		nGobs = 0;
		wireframe = false;
		iSelectedObject = 1;
		currentscene = 0;
		rm = &m_rm;//point to primary resourcemanager
		LoadScene(currentscene);
		counter = 0;
		screenScale = 1;
		if (thiz==nullptr) thiz = this;
		desktop = false;//on desktop platforms sdk will set it to true 
		#ifndef NO_FONTLIB
		if (!(font = dtx_open_font_glyphmap("serif_s24.glyphmap"))) {
			fprintf(stderr, "failed to open font\n");
		} else
		dtx_use_font(font, 24);
		#endif
            
#ifdef fopen_s
		fopen_s(&f, "AlgeApp.log", "w");
#else
        f = fopen("AlgeApp.log", "w");
#endif
	}
		
	CModel* fontModel;
	char sz1[128];

#define NO_FONTLIB

    void Enable(int FEATURE) {
        output.pushF(FEATURE);
    }
    
	virtual void onNatsServiceResponce(string service, int id, string responce) {
	
	}

	virtual void onWidgetEvent(GameObject* sender, PEG::CMD* p) {

	}

	void Init2DWithJpgBackground(std::string title, GameObject* bg, std::string bkg) {
		AlInit(STANDARD);
		//game.AddDefaultCamera(Camera::CAM_MODE_2D, OrthoTypes::ORIGIN_IN_TOP_LEFT_OF_SCREEN);
		AddDefaultCamera(Camera::CAM_MODE_2D, OrthoTypes::ORIGIN_IN_TOP_LEFT_OF_SCREEN);
		AddResource(bg, bkg, bkg + ".jpg", XFunction_AutoScalingToFullScreen::AUTO_SCALING_FULLSCREEN);
	}

    virtual bool onEventTap() {
        return true;
    }
    
    virtual bool onKeySpec(int v, int p, int k) {
        return true;
    }
    
	string LoadPreference(string key) {
		FILE* f;
#ifndef fopen_s
        f = fopen((key + ".cfg").c_str(), "r");
#else
        fopen_s(&f, (key + ".cfg").c_str(), "r");
#endif
		char sz[128] = { 0 };
		if (f) {

			fgets(sz, 128, f);
			fclose(f);
		}
		return string(sz);
	}

	void SavePreference(string key, string value) {
		FILE* f;
#ifndef fopen_s
        f = fopen((key + ".cfg").c_str(), "w");
#else
		fopen_s(&f, (key + ".cfg").c_str(), "w");
#endif
		if (f) {
			fputs(value.c_str(), f);
			fclose(f);
		}
	}

	std::vector<std::string> GetAllFileNames(const std::string& pat) {
		std::vector<std::string> ret;
#ifdef IOS
        return ret;
#else
        //This block is compat with IOS 13+ not 12.5.7
		namespace fs = std::filesystem;
		try {
			for (const auto& entry : fs::directory_iterator(pat)) {
				auto a = entry.path();// .extension();
				{
					ret.push_back(entry.path().string());
				}
			}
		}
		catch (...) {}
		return ret;
#endif
	}

	std::vector<std::string> GetModelNames(const std::string& pat) {
		std::vector<std::string> ret;
		
#ifndef IOS
		//This block is compat with IOS 13+ not 12.5.7
		namespace fs = std::filesystem;
		try {
			for (const auto& entry : fs::directory_iterator(pat)) {
				auto a = entry.path().extension();
				if (a == ".alx" || a == ".obj") {
					ret.push_back(entry.path().string());
				}
			}
		}
		catch (...) {}
#endif
		return ret;
	}

	std::vector<std::string> GetFileNames(const std::string& pat, const std::string& extension) {
		std::vector<std::string> ret;
#ifndef IOS
        //This block is compat with IOS 13+ not 12.5.7
		namespace fs = std::filesystem;
		try {
			for (const auto& entry : fs::directory_iterator(pat)) {
				auto a = entry.path().extension();
				if (a == "." + extension) {
					ret.push_back(entry.path().string());
				}
			}
		}
		catch (...) {}
#endif
		return ret;
	}
    
    string pathPrefix() {
        string prefix = "../../Apps/";
#ifndef WIN32
        prefix = "/Users/aliveb/REPOS/XP08-AlgeSDK/Apps/";
#endif
        return prefix;
    }
    
	std::vector<std::string> GetAppSceneNames(const std::string& pat) {
		std::vector<std::string> ret;
#ifndef IOS
        //This block is compat with IOS 13+ not 12.5.7
		namespace fs = std::filesystem;

		try {
			for (const auto& entry : fs::directory_iterator(pathPrefix())) {
				auto a = entry.path();// .extension();
				if (a.generic_string().find(".xsn") != string::npos) {
					string str = entry.path().string();
					std::replace(str.begin(), str.end(), '\\', '/');
					try {
#ifdef WIN32
						string ap = split(split(str, '.')[4], '/')[2];
#else
						string apv = split(str, '.')[0];
						vector<string> apvn = split(apv, '/');
						string ap = apvn[apvn.size() - 1];
#endif
						ret.push_back(ap);
					}
					catch (...) {}
				}
			}
		}
		catch (...) {}
#endif
		return ret;
	}

	std::vector<std::string> GetPeerAppNames(const std::string& pat) {
		std::vector<std::string> ret;
		namespace fs = std::filesystem;
#ifndef IOS
        try {  //This block is compat with IOS 13+ not 12.5.7
            for (const auto& entry : fs::directory_iterator(pathPrefix())) {
                auto a = entry.path();// .extension();
                if (a.generic_string().find(".Assets")!=string::npos) {
                    string str = entry.path().string();
                    std::replace(str.begin(), str.end(), '\\', '/');
                    try {
#ifdef WIN32
                        string ap = split(split(str, '.')[4], '/')[2];
#else
                        string apv = split(str, '.')[0];
                        vector<string> apvn = split(apv, '/');
                        string ap = apvn[apvn.size()-1];
#endif
                        ret.push_back(ap);
                    }
                    catch (...) {}
                }
            }
        } catch (...) {}
#endif
		return ret;
	}


#ifndef NO_IMGUI
	ImFont* UseTtf(const char* ttf, float siz) {
		ImFont* afont = ImGui::GetIO().Fonts->AddFontFromFileTTF(
			ttf, siz
		);
		return afont;
	}
#endif // !NO_IMGUI


	string commandline;

	void SetCommandLine(string l) { commandline=l; 
		input.pushP(CMD_COMMANDLINE, (char*)commandline.c_str(), 0);
	}

	static string touched_bodies_list;

	PosRotScale* touched_body_top() {
		PosRotScale* ret = nullptr;
		touched_bodies_list = "#";
		for (auto t : touched_bodies) {
			f3 p = t->GetPos();
			CRect cr = t->getOwnRect();
			if (ret != nullptr && !ret->hidden ) {
				t->UUID = t->UUID;
				touched_bodies_list += t->UUID + ",";
			}
			ret = t;
		}
		if (ret != nullptr) return ret;
		return nullptr;
	}


	int getPreferredTitleCorrection() {
#ifdef ANDROID
		return 80;// A visually appealing Size tested on Note10+ for Roboto-Bold
#endif
#ifdef WIN32
		return 0;//32
#endif
#ifdef IOS
        return 64;
#endif
#ifdef MACOS
		return 32;
#endif
	}

	float getPreferredScale() {
		float reference_hight_of_developer_station_main_window = 1125;
		float scale = coord.getResolutionForGameEngine().y / reference_hight_of_developer_station_main_window;

#ifdef WIN32
		scale = 1.;
#endif

#ifdef ANDROID
		scale *= 5;
#endif

		return scale;
	}

	string preferred_font_filename = "Roboto-Bold.ttf";
	const char* getPreferredFontFilename() { return preferred_font_filename.c_str(); }
	
	virtual bool hideOnStartup() {
		return false;
	}

	virtual bool isDesktopApp() {
		return false;
	}

    int getPreferredTextSize() {
		short sz = 48;
#ifdef ANDROID
        sz =  48;// A visually appealing Size tested on Note10+ for Roboto-Bold
#endif
#ifdef WIN32
        sz = 48;
#endif
#ifdef IOS
        sz = 48;
#endif
#ifdef MACOS
        sz = 48;
#endif

		if (isDesktopApp()) sz = sz / 0.75;

		return sz;
    }

	virtual bool isLandscape() {
		return false;
	}

	virtual i2 getBackgroundSize() {
		return i2(rightSide, bottomSide);
	}

	i2 getResolutionForGuiUsage() {
		return size_note10.flip();
	}

	std::string_view ltrim(std::string_view s)
	{
		s.remove_prefix(std::distance(s.cbegin(), std::find_if(s.cbegin(), s.cend(),
			[](int c) {return !std::isspace(c); })));

		return s;
	}

	std::string_view rtrim(std::string_view s)
	{
		s.remove_suffix(std::distance(s.crbegin(), std::find_if(s.crbegin(), s.crend(),
			[](int c) {return !std::isspace(c); })));

		return s;
	}

	std::string_view trim(std::string_view s)
	{
		return ltrim(rtrim(s));
	}

	string toUpperCase(string str) {
		std::transform(str.begin(), str.end(), str.begin(), ::toupper);
		return str;
	}

	string replaceAll(string full, string what, string str) {
		return std::regex_replace(full, std::regex(what), str);
	}


    virtual short getOrientation() {
		return Coord::ORIENTATION_PORTRAIT;
    }
    
	void adjustWindowSize(int w, int h) {
		appSize(w, h, 1);
	}

	void MoveObjectLeftRightAsPerTilt(f3* accel100, GameObject* o,int speed, int extent = 5) {
		int a = accel100->y * 9 ;//Scale -10 to  -90 and +10 to 90
		bool intentLeft = (a<-5);
		bool intentRight= (a>5);
		if (!intentLeft && !intentRight) return;
		f3 hp = o->GetPos();
		PosRotScale nprs;
		f3 np;
		np.z = hp.z;
		np.y = hp.y;
		if (intentRight) np.x = hp.x  + extent ;
		if (intentLeft) np.x = hp.x  - extent ;
		nprs.SetPos(np.x, np.y, np.z);
		//	o->transitionTo(np, speed * abs(a));
	}

	f3 accel;

	virtual void onAcceclerometerValues(f3 f_accel) {
		accel.x = f_accel.x;
		accel.y = f_accel.y;
		accel.z = f_accel.z;
	}

	float InitScale(int screen_width, int works_best) {
		int works_best_on_screen_width = works_best;
		return 1.0 * screen_width / works_best_on_screen_width;
	}



	bool onPause() {
		paused = true;
		return true;
	}

	bool onResume() {
		paused = false;
		return true;
	}

	inline void alPrint(const char* text,  int size = 24 ) {
		#ifndef NO_FONTLIB
		if (size!=24) dtx_use_font(font, size);
		glPushMatrix();
		
		glRotatef(180, 1, 0, 0);
		dtx_string(text);
		glPopMatrix();
		#else
		glPushMatrix();
		glRotatef(180, 1, 0, 0);
		strcpy(sz1, text);
	//	fonts.print(sz1);
		glColor3f(0.5, 0.5, 0.5);
		glScalef(15,15,15);
	//	fontModel->glDraw();
		glPopMatrix();
		glColor3f(1., 1., 1.);
		#endif
	}

	void Deinit() {
		if (f) fclose(f);
	}
	
	
	void setTiming(float timeMultiplier) {
		this->timeMultiplier = timeMultiplier;
	}

	vector<bool> dimmed_states;

	//caution: No New objects should be added between the Dim and Undim call
	void Dim() {
		int i0 = 0;
		for (int i = 0; i < nGobs; i++) {
			dimmed_states.push_back(gobs[i]->hidden); 
			if (gobs[i]->modelId != backgroundModelId) gobs[i]->hidden = true;
			for (unsigned short j = 0; j < gobs[i]->prsInstances.size(); j++) {
				dimmed_states.push_back(gobs[i]->prsInstances[j].hidden);
				gobs[i]->prsInstances[j].hidden = true;
				i0++;
			}
			i0++;
		}
		paused = true;
	}

	void UnDim() {
		if (dimmed_states.size()<=0) return;
		int i0 = 0;
		for (int i = 0; i < nGobs; i++) {
			gobs[i]->hidden = dimmed_states[i0];
			for (unsigned short j = 0; j < gobs[i]->prsInstances.size(); j++) {
				gobs[i]->prsInstances[j].hidden = dimmed_states[i0];
				i0++;
			}
			i0++;
		}
		dimmed_states.clear();
		paused = false;
	}
	
	virtual void onSettingChanged(string setting, int value) {
		static char msg[128];
		static int _value = value;
		strcpy(msg, setting.c_str());
		output.pushP(CMD_SETTINGS_SCREEN, $ msg , (void*) &_value);
	}
//	i2 primarytarget;


//	virtual i2 getBackgroundSize() {
//		//dev inventory
//		i2 a51(2400, 1080);
//		i2 a51p(1080, 2400);
//		i2 ipad(2048, 1536);
//		i2 iphone6(1334, 750);
//		i2 iphone5(1136, 640);
//		//in use dev devices
//		i2 appledevice = iphone5;
//		i2 androiddevice = a51p;
//		//focus on
//		primarytarget = androiddevice;
//		i2 ret;
//#ifdef ANDROID
//		ret = androiddevice;
//#endif
//#ifdef IOS
//		ret = appledevice;
//#endif
//#ifdef MACOSX
//		ret = primarytarget;
//#endif
//#ifdef WIN32
//		i2 cinema_display(1920, 1200);
//		float yscale = float(cinema_display.y) /float(androiddevice.y) * 0.95; //0.95 is to cater for titlebar
//		i2 android_on_cinema_display(a51p.x * yscale, a51p.y * yscale);// reduce both  x and y according to y
//		ret = android_on_cinema_display;
//		primarytarget.y = 2000;
//		primarytarget = cinema_display;
//		ret = primarytarget;
//#endif
//	//	if (!landscape) ret = i2(ret.y,ret.x);
//		return ret;
//	}

	virtual void onActionComplete(GameObject* obj) {
		
	}

    virtual void RenderGui(float deltaT) {
        
    }
    
	//Preconditions: Dont call before init
	//Assumption Scene is Saved from same arrangement of Gobs
	short LoadScene(short n) {
		/*char filename[16];
		 sprintf(filename, "")
		 for (int i=0; i<nGobs; i++) {
		 gobs[i]
		 }
		 */
		return 0;
	}

	GameObject* AddObject(GameObject *o, GameObject* parent = nullptr) {
		nGobs++;
		if (nGobs >= 128) nGobs = 127;
		gobs[nGobs - 1] = o;
		if (parent) parent->AddChild(o);
#ifndef NO_NATS
		netmsg.Post(string("AddObject::") + o->Name());
#endif
		if (o->UUID.size()<1) o->UUID = "GOB#" + to_string(nGobs - 1);
		return o;
    }
	int orthoType = OrthoTypes::ORIGIN_IN_TOP_LEFT_OF_SCREEN;

	bool doObjectsIntersect(PosRotScale* prsCircle, PosRotScale* prsRect) {
		f3 p = prsCircle->GetPos();
		f3 q = prsRect->GetPos();
		return isCircleIntersectingRect(p.x, p.y, prsCircle->m_width /2, q.x, q.y, prsRect->m_width/2 , prsRect->m_height/2 );
	}

	struct Polygon {
		vector<float> Points;
	};

	
	void PositionCamera(f3 position, f3 rotation) {
        SF3dVector p, r;
        p.x = position.x;
        p.y = position.y;
        p.z = position.z;
        r.x = rotation.x;
        r.y = rotation.y;
        r.z = rotation.z;
		aCamera.PosRot(p,r);
		aCamera.SetPos(position);
		aCamera.rot = rotation;
	}
    
	void AddDefaultCamera(int camMode = Camera::CAM_MODE_FPS, int _orthoType= ORIGIN_IN_TOP_LEFT_OF_SCREEN) {
		orthoType = _orthoType;
		static bool once = false;
		if (!once) {
			aCamera.SetSerializeTag("ACAMERA.GOB");
			aCamera.SetMode(camMode);
			AddObject(&aCamera);
			once = true;
		}
        SetCamera(camMode, _orthoType);
        
        
	}

    void CamReshape(i2 rect) {
        aCamera.windowWidth = rect.x;
        aCamera.windowHeight = rect.y;
		
		if (orthoType == ORIGIN_IN_MIDDLE_OF_SCREEN) {
            leftSide = -aCamera.windowWidth / 2.0;
            rightSide = aCamera.windowWidth / 2.0;
            bottomSide = -aCamera.windowHeight / 2.0;
            topSide = aCamera.windowHeight / 2.0;
            originX = 0;
            originY = 0;
        }
        if (orthoType == ORIGIN_IN_TOP_LEFT_OF_SCREEN) {
            leftSide = 0;
            rightSide = aCamera.windowWidth;
            bottomSide = aCamera.windowHeight;
            topSide = 0;
            originX = rightSide / 2;
            originY = bottomSide / 2;
        }
    }
    
    void SetCamera(int camMode = Camera::CAM_MODE_FPS, int _orthoType= ORIGIN_IN_TOP_LEFT_OF_SCREEN) {
        orthoType= _orthoType;
        aCamera.mode = camMode;
        CamReshape(coord.getResolutionForGameEngine());
    }
	//float adjustScale(float scale) {
	//	float m_scale = -1;
	//	if (scale != XFunction_AutoScalingToFullScreen::AUTO_SCALING_FULLSCREEN) {
	//		m_scale = scale * InitScale(getBackgroundSize().x, best_width);
	//	}
	//	else {
	//		m_scale = scale;
	//	}
	//	return m_scale;
	//}
	
	// .jpg has no alpha channel, for alpha channel support use tga pair
	GameObject* AddResourceEx(GameObject* g, string tag, int numInstances_max99, bool is_circle = false, float scale = 1.0, float density = 1.0, float restitution = 0.1) {
		AddResource(g, tag, tag, scale);
		g->UUID = tag;
		if (g->m_height>0 && g->m_width>0)
			AddMultiplePhysicalInstances(g, numInstances_max99, is_circle, density, restitution, scale); //physics require half width/ half height
		return g;
	}


	int best_width = 1200;
	GameObject* AddResource(GameObject* g, string tag, float scale = 1.0) {
		float m_scale = scale;//adjustScale(scale);
		g->scale = scale;
		GameObject* gs = AddResource(g, tag, tag, m_scale);
		gs->m_height = scale * gs->m_height;
		gs->m_width = scale * gs->m_width;
		gs->scale = m_scale;
		gs->mScale[0] = scale;
		gs->mScale[1] = scale;
		gs->mScale[2] = scale;
		gs->originalScale = scale;
		return gs;
	}

	//Used to scale e.g set this to your background image
	GameObject* refObject;



	//Assume tag as name of alx(without extension) and tex is tga(without extension)/jpeg(with extension)
	//Example AddResource(&bg, "background", "background.jpg", 1.0);
	GameObject* AddResource(GameObject* g, string alx_name, string tex_name, float scale=1.0) {
		ResourceInf res;
		bool jpeg = (tex_name.find(".jp") != string::npos);
		
		
		res.scale = scale;

		if (refObject)
			if (refObject->modelId>0)
				refObject->setTexWidth(rm->models[refObject->modelId]->originalWidth());
		if (refObject)
			if (refObject!=g)
				res.scale = scale * coord.getScaleOfOjects(refObject);

		bool isObj = (alx_name.find(".obj") != string::npos);

		if (isObj) {
			res.Set(string(tex_name), string(alx_name), string(tex_name), res.scale);
		} else {
			res.Set(string(tex_name), string(alx_name + ".alx"), string(tex_name + (jpeg ? "" : ".tga")), res.scale);
		}
		


		g->modelId = LoadModel(g, &res);
		TRAP(g, "earth");
		AddObject(g);
		float g_pos_x = aCamera.windowWidth/2;
		float g_pos_y = aCamera.windowHeight/2;
		g->SetPos(g_pos_x, g_pos_y, 0);
		g->originalScale = scale;
		g->JuiceType = 0;
		f3 b = rm->models[g->modelId]->bounds();
        g->SetBounds(b.x, b.y, tex_name);
		g->UUID += ":" + alx_name + ":";
        g->Text = g->UUID;
		XFunction_AutoScalingToFullScreen::GameObjectAdded(g, scale);
		g->m_touched = false;
		return g;
	}
	

    GameObject* AddClusterResource(int n_duplicates, string alxtag, string tgatag, GameObject* array_of_objects, float size, int juiceType,bool useinstancing=false) {
        GameObject* g;
        if (!useinstancing) {
            for (int i = 0; i < n_duplicates; i++) {
                g = (array_of_objects+i);
                AddResource(g, alxtag,tgatag, size);
                g->SetPos(randm() * rightSide, randm() * bottomSide);
                g->applyTopLeftCorrectionWRTorigin = false;
                g->JuiceType = juiceType;
            } } else {
                /*  INSTANCING :S NOT FUNCTIONAL PROPERLY YET
                 AddResource(&star, "star", 20);
                 star.pos.x = originX;
                 star.pos.y = originY;
                 star.NextJuice();
                 
                 PosRotScale starPrs;
                 starPrs.JuiceType = 0;
                 starPrs.JuiceSpeed = 10;
                 for (int i = 0; i < 10; i++) {
                 starPrs.pos.x = randm()* rightSide;
                 starPrs.pos.y = randm()* bottomSide;
                 //starPrs.scale = star.scale;
                 star.AddInstance(starPrs);
                 printf("Star(%.1f,%.1f)", starPrs.pos.x, starPrs.pos.y);
                 }*/
            }
        return array_of_objects;
    }
    
	int leftSide, rightSide, topSide, bottomSide, originX, originY;

#ifdef WIN32
#define glOrthof glOrtho
#endif

	void ViewOrthoBegin(float left, float right, float bottom, float top, float depth = 1)												// Set Up An Ortho View
	{
		glDisable(GL_DEPTH_TEST);							// Disables Depth Testing
		glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
		glPushMatrix();										// Store The Projection Matrix
		glLoadIdentity();									// Reset The Projection Matrix
		glOrthof(left, right, bottom, top, -depth, depth);
		glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
		glPushMatrix();										// Store The Modelview Matrix
		glLoadIdentity();									// Reset The Modelview Matrix
	}

	void ViewOrthoBegin(int depth = 1)												// Set Up An Ortho View
	{
		glDisable(GL_DEPTH_TEST);							// Disables Depth Testing
		glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
		glPushMatrix();										// Store The Projection Matrix
		glLoadIdentity();									// Reset The Projection Matrix
		glOrthof(leftSide, rightSide, bottomSide, topSide, -depth, depth);
		glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
		glPushMatrix();										// Store The Modelview Matrix
		glLoadIdentity();									// Reset The Modelview Matrix
	}

	void ViewOrthoEnd()											// Set Up A Perspective View
	{
		glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
		glPopMatrix();										// Restore The Old Projection Matrix
		glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
		glPopMatrix();										// Restore The Old Projection Matrix
		glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	}

	void UndoInheritedRotation(GameObject* it) {
		it->rot.x = -it->parent->rot.x;
		it->rot.y = -it->parent->rot.y;
		it->rot.z = -it->parent->rot.z;

		//https://stackoverflow.com/questions/22269435/how-to-iterate-through-a-list-of-objects-in-c
		for (auto const& child : it->children) {
			child->rot.x = -it->rot.x;
			child->rot.y = -it->rot.y;
			child->rot.z = -it->rot.z;
		}
	}

	void ammendTopLeft2D(f3* pos, float scaledTo, float widthOriginal) {
		float w = widthOriginal / scaledTo;
		pos->x += 0.5 * w * (1 - scaledTo);
		pos->y += 0.5 * w * (1 - scaledTo);
	}

	int juice_frame[JuiceTypes::JUICES_END];
	
	void UpdateJuices(GameObject* it, int instanceNo, float deltaT) {
	//	static float juice_sine;
		
		static float elapsed = 0;
		
		elapsed += deltaT;
	//	static float timeNote;
	//	static bool timeNoted = false;
		static float x_pos_on_arrival = -1;

		PosRotScale* jprs = (instanceNo < 0) ? reinterpret_cast<PosRotScale*>(it) : (it->getInstancePtr(instanceNo));

		float speedDt = (deltaT * (jprs->JuiceSpeed * 100));
		float angleDt = speedDt * 0.1;

		switch (jprs->JuiceType) {

		case JuiceTypes::JUICE_ROTZ:
			jprs->rot.z += speedDt;
			break;
		case JuiceTypes::JUICE_ROTY:
			jprs->rot.y += speedDt;
			break;
        case JuiceTypes::JUICE_ROTX:
            jprs->rot.x += speedDt;
            break;
        case JuiceTypes::JUICE_FLICKER:
             //jprs->scale *= ;
             break;
		case JuiceTypes::JUICE_DIE_TEMP:
		case JuiceTypes::JUICE_DIE:
			if (jprs->JuiceDuration>0) {
				jprs->rot.z += speedDt;
				jprs->juice_sine_angle += 0.01;/// (0.5f * jprs->JuiceSpeed);
				if (jprs->juice_sine_angle >= 6.28319) jprs->juice_sine_angle = 0.0; //rotate on 360 degree i.e 6.28319 radians
				float fsin = float(1.0 + 0.2 * sin(jprs->juice_sine_angle));
				glScalef(fsin, fsin, fsin);
				jprs->JuiceDuration -= deltaT;
			}	else {
				if (jprs->JuiceType == JUICE_DIE) {
					jprs->hidden = true;
					jprs->JuiceType = 0;
					jprs->rot.z = 0;
					jprs->juice_sine_angle = 0.;
				}
			}
			break;
		case JuiceTypes::JUICE_ROTXYZ:
			jprs->rot.x += speedDt;
			jprs->rot.y += speedDt;
			jprs->rot.z += speedDt;
		break;
		case JuiceTypes::JUICE_ROTXYZ_PULSATE_FULLY:
		{
			jprs->rot.x += speedDt;
			jprs->rot.y += speedDt;
			jprs->rot.z += speedDt;
			jprs->juice_sine_angle += angleDt / 2.;
			float sinj = sin(jprs->juice_sine_angle);
			glScalef(abs(sinj), abs(sinj), abs(sinj));
			break;
		}
		case JuiceTypes::JUICE_ROTZ_PULSATE:
		{
			jprs->rot.z += (deltaT * (jprs->JuiceSpeed * 100));
			jprs->juice_sine_angle += angleDt;
			float sinjx = 1. + 0.02 * sin(jprs->juice_sine_angle);
			glScalef(sinjx, sinjx, sinjx);
			break;
		}
		case JuiceTypes::JUICE_PULSATE:
		{
			jprs->juice_sine_angle += angleDt;
			//jprs->rot.z += (deltaT * jprs->JuiceSpeed);
			float sinjx = 1. + 0.02 * sin(jprs->juice_sine_angle);
			glScalef(sinjx, sinjx, sinjx);
			break;
		}
		
		//for this to work also set jprs->scale = 0 to start effect
		case JuiceTypes::JUICE_SCALE_IN:
		{
			if (jprs->scale < jprs->originalScale) {
				jprs->scale *= 1.02;
			} else {
				jprs->scale = jprs->originalScale;
			}
			break;
		}

		case JuiceTypes::JUICE_SCALE_OUT:
		{
			if (jprs->scale > 0) {
				jprs->scale /= 1.02;
			}
			else {
				jprs->scale = jprs->originalScale;
			}
			break;
		}

		case JuiceTypes::JUICE_PULSATE_FULLY:
		{
			//jprs->rot.z += (deltaT * jprs->JuiceSpeed);
			jprs->juice_sine_angle += angleDt / 2.;
			float sinj = sin(jprs->juice_sine_angle);
			glScalef(abs(sinj), abs(sinj), abs(sinj));
			break;
		}
		case JuiceTypes::JUICE_FLY_OUT:
			
			if (jprs->GetPos().x > -999) {
				if (x_pos_on_arrival==-1) x_pos_on_arrival = jprs->GetPos().x;
				f3 j = jprs->GetPos();
				j.x -= speedDt * 50;
				if (j.x < (-x_pos_on_arrival)) {
					j.x = x_pos_on_arrival;
					jprs->JuiceType = JuiceTypes::JUICES_CANCEL;
					jprs->juice_sine_angle = 0;
					x_pos_on_arrival = -1;
				}
				jprs->SetPos(j);
			}
			else {
				//jprs->pos.x = x_pos_on_arrival;
				//jprs->JuiceType = 0;
				//x_pos_on_arrival = -1;
				//jprs->hidden = false;
			}
			break;
		
		case JuiceTypes::JUICES_CANCEL:
			jprs->rot.x = 0;
			jprs->rot.y = 0;
			jprs->rot.z = 0;
			jprs->juice_sine_angle = 0;
			jprs->hidden = false;
			break;
		}


	}

	void renderSingleObject(GameObject* iit, float deltaT = 0.1f, int instanceNo = -1) {

		if (iit->hud) hudBegin(0, rightSide, bottomSide, 0);

		PosRotScale* it = iit->getInstancePtr(instanceNo);
		if (iit->modelId>=0)
			rm->models[iit->modelId]->is3D() ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
	//	static float wobble = 0.;
		glPushMatrix();
		f3 relPos, relRot;
	
		relPos = it->GetPos();

		relRot.x = it->rot.x;
		relRot.y = it->rot.y;
		relRot.z = it->rot.z;

		//glScalef(it->scale, it->scale, it->scale);

		if (iit->parent) {
			relPos.x += iit->parent->pos.x;
			relPos.y += iit->parent->pos.y;
			relPos.z += iit->parent->pos.z;
			relRot.x += iit->parent->rot.x;
			relRot.y += iit->parent->rot.y;
			relRot.z += iit->parent->rot.z;
		}

		if (!iit->rotatefirst) glTranslatef(relPos.x, relPos.y, relPos.z);

		glRotatef(relRot.x, 1., 0., 0.);
		glRotatef(relRot.y, 0., 1., 0.);
		glRotatef(relRot.z, 0., 0., 1.);

		if (iit->rotatefirst) glTranslatef(relPos.x, relPos.y, relPos.z);

		if (it->hidden) return;
		
		glScalef(it->scale, it->scale, it->scale);
		
        
		if (edit) {
			if (iit->modelId >= 0)
			xyz.setSize(rm->models[iit->modelId]->bounds().z * 200.f);
			xyz.glDraw();//draw axis
		   //xyz.glDrawGroundAsGrid();
		}


		//it->Update(deltaT);
		if (iit->modelId == backgroundModelId) {
			iit->color = (dimmed_states.size() > 0) ? f3(0.5, 0.5, 0.5) : f3(1, 1, 1);
		}
		
		glColor3f(it->color.x, it->color.y, it->color.z);
		if (iit != &aCamera) UpdateJuices(iit, instanceNo, deltaT);
		
		UpdateCustom(iit, instanceNo, deltaT);
		
//	if (instanceNo>0 && it->applyTopLeftCorrectionWRTorigin) {//
//			PosRotScale* i = iit->getInstancePtr(instanceNo);
//			i->pos.x = originX;
//			i->pos.y = originY;
//			float originalWidth = rm.models[iit->modelId]->originalWidth();
//			ammendTopLeft2D(&i->pos, i->scale, originalWidth);
//		}
        
		if (iit->billboard) alBillboardBegin();
        
        if (it->originalScale == XFunction_AutoScalingToFullScreen::AUTO_SCALING_FULLSCREEN) {
            if (getOrientation()==coord.ORIENTATION_LANDSCAPE) {
            float sx = rightSide ;
            float sy = bottomSide / it->originalAspect;
         	if (orthoType == OrthoTypes::ORIGIN_IN_TOP_LEFT_OF_SCREEN) {
				it->pos.x = rightSide /2;
				it->pos.y = bottomSide /2;
			} //else no need to adjust pos
            glScalef(sx, sy, 1);
            } else {
                float sy = rightSide ;
                float sx = bottomSide / it->originalAspect;
                 if (orthoType == OrthoTypes::ORIGIN_IN_TOP_LEFT_OF_SCREEN) {
                    it->pos.x = rightSide /2;
                    it->pos.y = bottomSide /2;
                } //else no need to adjust pos
                glScalef(sx, sy, 1);
            }
        } else
            glScalef(it->scale, it->scale, it->scale);
    

		int m_j = it->JuiceType;//save  *1 >>>>>
		if ((edit && it == gobs[iSelectedObject])) it->JuiceType = JuiceTypes::JUICE_PULSATE;

		
		if (edit) {
			xyz.glDraw();
		}
	
		if (iit->modelId >= 0 && !inhibitRender && !iit->hidden)
			alDrawModel(iit->modelId, wireframe);
		
		if (iit->hud) hudEnd();

		if (iit->billboard) alBillboardEnd();

		it->JuiceType = m_j;//restore *1 <<<<<

		//ShowMarkerinOrthoMode(10);

		glPopMatrix();
		inhibitRender = false;
		glColor3f(1., 1., 1.);

	}
	
	static AlgeApp* thiz;
	static f3 mousepos;

	static void ImGuiText(string any) {
#ifndef NO_IMGUI
		ImGui::Text("%s",any.c_str());
#endif
	}

	
	static void InfGuiHover(float dt = 0.01, float windowFontScale = 1.0f) {
		if (mousepos.x == 0 && mousepos.y == 0) return;
		if (thiz->rightSide == 0 || thiz->bottomSide == 0) return;
#ifndef NO_IMGUI

		static string lasttop = "";
	//	ImGui::SetNextWindowSize(ImVec2(thiz->rightSide,thiz->bottomSide / 2));
		ImGui::SetNextWindowBgAlpha(0.5);
		ImGui::Begin("InfGuiHover");
		SetWindowFontScale(windowFontScale);
		static bool hidehidden = false;
		static bool unhideall = false;
		f3 center, dim;
		center.x = thiz->rightSide / 2;
		center.y = thiz->bottomSide / 2;
		dim.x = thiz->rightSide;
		dim.y = thiz->bottomSide;
		ImGuiText(center.str("Center:%.f,%.f") + " of " + dim.str("%.f,%.f"));
		ImGuiText(mousepos.str("Mouse:%.f,%.f"));
		ImGui::Checkbox("Hide Hidden", &hidehidden);
		ImGui::SameLine(); ImGui::Checkbox("Wireframe", &thiz->wireframe);
		ImGui::Checkbox("Unhide all (Disruptive)", &unhideall);
		
		for (int i = 0; i < thiz->nGobs; i++) {
			bool hidethis = false;
			if (unhideall) thiz->gobs[i]->hidden = false;
			hidethis = hidehidden && thiz->gobs[i]->hidden;
			hidethis = hidethis || thiz->gobs[i]->prsInstances.size() > 0;
			
			if (!hidethis)
			ImGuiText((
				thiz->gobs[i]->toString() +
				(thiz->doPicking2D(thiz->gobs[i], AlgeApp::mousepos.xy()) ? "<HOVER" : "") +
				((thiz->gobs[i]->hidden) ? "<HIDDEN" : "")
				))
				;
			int ii = 1;
			for (auto p : thiz->gobs[i]->prsInstances) {
				if (unhideall) p.hidden = false;
				hidethis = hidehidden && p.hidden;
				if (!hidethis)
				ImGuiText((
					" "+ thiz->gobs[i]->getInstancePtr(ii)->toString() +
					" #" + to_string(ii) +
					(thiz->doPicking2D(&p, AlgeApp::mousepos.xy()) ? "<HOVER" : "") +
					((p.hidden) ? "<HIDDEN" : "")
					))
					;
				ii++;
			}
		}

		ImGuiText("Clicked:"+clickedbody);
		ImGui::SameLine();
		ImGui::End();

#endif // !NO_IMGUI
	}

	static string clickedbody;

	static string Clicked() {
		string ret = "";
		if (thiz->touched_body_top()) {
			ret = thiz->touched_body_top()->GetName();
			thiz->clickedbody = ret;
		}
		return ret;
	}

    bool doPicking2D(PosRotScale* it, f2 mouse, float extent = 1.0) {
		if (rightSide == 0 || bottomSide == 0) return false;
        //if (it->touchable==false) return false;
      //  TRAP(it, "start");
		i2 fb = i2(rightSide, bottomSide);// coord.getResolutionForGameEngine();
		i2 rr = fb;// coord.getResolutionForGameEngine();
      //  if (coord.orientation==Coord::ORIENTATION_LANDSCAPE)  {
		//	rr.x = coord.getResolutionForGameEngine().y;
	//		rr.y = coord.getResolutionForGameEngine().x;
     //   }
        if (rr.x==0) rr.x = fb.x;
        if (rr.y==0) rr.y = fb.y;
		float strangefactor = 25 * fb.y / bottomSide;
		f2 pt_in_world = f2(mouse.x / rr.x * fb.x, mouse.y / rr.y * fb.y);// +strangefactor );
	   CRect obj = it->getOwnRect(it->UUID);
	   

		if (it->scale == XFunction_AutoScalingToFullScreen::AUTO_SCALING_FULLSCREEN) {
			obj.Top = topSide;
			obj.Bottom = bottomSide;
			obj.Right = rightSide;
			obj.Left = leftSide;
		}
		short ow = obj.Right - obj.Left;
		short oh = obj.Bottom - obj.Top;

	    bool ret = (CRect::PTInRect(pt_in_world.x -ow, pt_in_world.y - oh, obj, it->UUID));

		bool x1 = pt_in_world.x >= obj.Left;
		bool x2 = pt_in_world.x <= obj.Right;
		bool y1 = (pt_in_world.y) >= obj.Top;
		bool y2 = (pt_in_world.y) <= obj.Bottom;

		ret = x1 && x2 && y1 && y2;

        if (ret) {it->m_touchedX = pt_in_world.x; it->m_touchedY = pt_in_world.y;}
        return ret;
    }
    
    void TRAP(PosRotScale* prs, string name) {
        if (prs->UUID.find(name)!= string::npos) {
			"STEP OUT NOW";
        }
    }


    enum AdjustmentFlags {
	    AdjustNothing = 0x00,
	    CameraCutsScreenLeft50px = 0x01,
	};

	unsigned long Adjust = AdjustmentFlags::CameraCutsScreenLeft50px;


    PEG::CMD *adjustments(PEG::CMD *pCmd) {
		
		if (Adjust & AdjustmentFlags::CameraCutsScreenLeft50px) {
             if (pCmd->command==CMD_TOUCHMOVE || pCmd->command==CMD_TOUCH_START || pCmd->command==CMD_TOUCH_END ) //TODO add other touch cmd's
             {
                 pCmd->i1 -= 50;
             }
         }
         return pCmd;
    }

    void renderObjects(float deltaT, bool btrackball) {
		counter++;
		selectedObject = gobs[iSelectedObject];
		glPushMatrix();
		aCamera.Update(deltaT, selectedObject);

		if (btrackball) trackball.ApplyRotate();

		vec3 gray(0.5, 0.5, 0.5);
		xyz.setStippleAndColor(2, 0xAEAE, gray, gray, gray);

		if (edit) {
			xyz.glDraw();
			xyz.glDrawGroundAsGrid();
		}
        
        PEG::CMD* cmd = input.pull();
        preProcessInput(cmd, deltaT);


	
		touched_bodies.clear();
        		
        int picked = -1;
        
        for (int i = 1; i < nGobs; i++) {
			GameObject* it = gobs[i];
			if (inhibitRender) { inhibitRender = false; continue; }
            
            PosRotScale origPRS;
			origPRS.pos.x = it->pos.x; origPRS.pos.y = it->pos.y; origPRS.pos.z = it->pos.z;
			origPRS.rot.x = it->rot.x; origPRS.rot.y = it->rot.y; origPRS.rot.z = it->rot.z;
			origPRS.scale = it->scale;
			origPRS.m_height = it->m_height;
			origPRS.m_width = it->m_width;
			origPRS.JuiceSpeed = it->JuiceSpeed;
			origPRS.JuiceType = it->JuiceType;
			origPRS.hidden = it->hidden;

			int n = (int)it->prsInstances.size();
			bool instanced = (n > 0);

			for (int ins = 0; ins < n; ins++) {
				
				PosRotScale* prs = it->getInstancePtr(ins);
#ifndef NO_BOX2D
				if (prs->physBodyPtr) {
					b2Body *b = (b2Body*)prs->physBodyPtr;
					prs->pos.x = b->GetPosition().x * P2S;
					prs->pos.y = b->GetPosition().y * P2S;
					prs->rot.z = b->GetAngle() * 57.2958f;
				}
#endif
				it->pos.x = prs->pos.x; it->pos.y = prs->pos.y; it->pos.z = prs->pos.z;
				it->rot.x = prs->rot.x; it->rot.y = prs->rot.y; it->rot.z = prs->rot.z;
				it->scale = prs->scale;
				it->hidden = prs->hidden;
				it->m_width = prs->m_width;
				it->m_height = prs->m_height;
				it->JuiceSpeed = prs->JuiceSpeed;
				it->JuiceType = prs->JuiceType;
				
				//Touched flag updated here for instances
				if (cmd->command == CMD_TOUCH_START) {
                    
                    if (!it->hidden)
						if (doPicking2D(prs, f2(cmd->i1 
							, cmd->i2))) {
							touched_bodies.push_back(prs);
							picked = i; //will be overriden by last ordered object
							prs->m_touchedX = cmd->i1;
							prs->m_touchedY = cmd->i2;
						}
				}

				if (!prs->hidden) renderSingleObject(it, deltaT, ins);
			}

			if (!instanced && !it->hidden) {
#ifndef NO_BOX2D
				if (it->physBodyPtr) {
					b2Body *b = (b2Body*) it->physBodyPtr;
					it->pos.x = b->GetPosition().x *P2S;
					it->pos.y = b->GetPosition().y * P2S;
					it->rot.z = b->GetAngle() * 57.2958f;
				}
#endif
				renderSingleObject(&(*it), deltaT, -1);
				
				//Touched flag updated here for principal objects
				if (cmd->command == CMD_TOUCH_START) {
					if (!it->hidden)
						if (doPicking2D(it, f2(cmd->i1, cmd->i2))) {
							touched_bodies.push_back(it);
							//MessageBeep(MB_OK);
							picked = i; //will be overriden by last ordered object
							it->m_touched = true;
							it->m_touchedX = cmd->i1;
							it->m_touchedY = cmd->i2;
						}
				}
			} else {
				//it->pos.x = origPRS.pos.x; it->pos.y = origPRS.pos.y; it->pos.z = origPRS.pos.z;
				it->SetPos(origPRS.GetPos());
				it->SetRot(origPRS.GetRot());
				it->scale = origPRS.scale;
				it->hidden = origPRS.hidden;
				it->SetDim(origPRS.GetDim());
				it->JuiceSpeed = origPRS.JuiceSpeed;
				it->JuiceType = origPRS.JuiceType;
			}

			//netmsg.Post("Render::=" + it->Name());//Disabled due to perf concerns
		}


		glPopMatrix();

#ifndef  NO_IMGUI
        GuiStarts(); RenderGui(deltaT); GuiEnds();
#endif //  NO_IMGUI
        
        
		//allow for touch processing call onTouched(uuid) to determine if body is touched
		if (cmd->command>0)
			processInput(adjustments(cmd), deltaT);
		
		bool postTouchData = true;
		if (postTouchData) {
			if (cmd->command == CMD_TOUCH_START) {
				td.x = cmd->i1; td.y = cmd->i2;
				ostringstream oss;
				oss << "[TOUCHED x:" << cmd->i1 << " y:" << cmd->i2;
#ifdef NATS_EXTERN
				netmsg.Post(oss.str());
#endif
			}
			if (cmd->command == CMD_TOUCH_END) {
				ostringstream oss;
				CRect r(td.y, cmd->i2, td.x, cmd->i1);//TODO BUG for DRAG 
				oss << r.toStr();
#ifdef NATS_EXTERN
				netmsg.Post( "[" + oss.str() + "]]"); 
#endif
				td = i2( 0,0 );
			}
		}

	}
	i2 td;//used by postTouchData

	bool onTouched(string name) {
		//PosRotScale* t = touched_body_top();
		//string touched_uuid = "";
		//if (t) 
		//	touched_uuid = t->UUID;
		//bool match = false;
		//if (t) (t->UUID.find(name) != std::string::npos);
		//return match;

        if (touched_bodies.size() > 0) {
			PosRotScale* bk = touched_bodies.back();
			
			if (bk->UUID.find(name) != string::npos) {
				return true;
			}
           
		}
		return false;
	}


    short iSelectedObject = 1;
    
    void ShowMarkerinOrthoMode(float size = 100.0) {
        aL10 al;
        al.alBegin(GL_TRIANGLES);                                // Drawing Using Triangles
        al.glVertex3f(0.0f, size, 0.0f);                    // Top
        al.glVertex3f(-size, -size, 0.0f);                    // Bottom Left
        al.glVertex3f(size, -size, 0.0f);                    // Bottom Right
        al.alEnd();                                            // Finished Drawing The Triangle
    }

	char PlatformCode;

	//Called by alge-main cpp
	void Init0(char* path, char platcode) {
		edit = false;
		PlatformCode = platcode;

#ifndef NO_NATS
		try {
	//		netmsg.Post(help);
		}
		catch (...) {
			printf("Could not Post netmsg");
		}
#endif
		aCamera.custom_type = 0xCA;
        
		aCamera.windowWidth = coord.getResolutionForGameEngine().x;
		aCamera.windowHeight = coord.getResolutionForGameEngine().y;
		rm->Init(path);
		Init(path);
		//Font v1 init
		fontModel =  rm->loadAlxModel((char*) "font.alx", AUTO, alReserveModelId(), 1);	// Fonts Loaded just like Model load but it doesnt involve managed resource 
		fontsV1.usetexof(fontModel);						// Associate texure of Loaded Model to FontMap
#ifndef NO_IMGUI
	//	if (path[0] == 0) MessageBoxA(NULL, "path is null", "AlgeApp::1942", MB_ICONASTERISK);
		
		imHelper.Init(path, "consola.ttf", i2(aCamera.windowWidth, aCamera.windowHeight));
#endif
	}


	virtual void Init(char* path) {};
	virtual void RenderGui() {};
	virtual void UpdateCustom(GameObject*, int, float) {};
	virtual void Update(float delta_t) {};
	virtual void processInput(PEG::CMD* p, float deltaT) {};

	struct DirectionMagnitude {
		char directionX; //L R
		char directionY; //U D
		float magnitudeX;
		float magnitudeY;
	};

	DirectionMagnitude getMouseIntent(int newX, int newY, int oldX, int oldY) {
		static DirectionMagnitude dm;
		dm.directionX = ' ';
		dm.directionY = ' ';
		if (newX > oldX) dm.directionX = 'R'; else dm.directionX = 'L';
		if (newY > oldY) dm.directionY = 'D'; else dm.directionY = 'U';
		if (newX == oldX) dm.directionX = ' ';
		if (newY == oldY) dm.directionY = ' ';
		static char msg[3];
		msg[0] = dm.directionX;
		msg[1] = dm.directionY;
		msg[2] = 0;
	//	output.pushP(CMD_TITLE, msg, 0);
		dm.magnitudeX = (newX - oldX) / 5.;
		dm.magnitudeY = (newY - oldY) / 10.;
		if (dm.directionX == ' ') dm.magnitudeX = 0.f;
		if (dm.directionY == ' ') dm.magnitudeY = 0.f;
		return dm;
	}
	
    ///////////////////////////////////////
	// Usage of remote commands thru telnet:
	// ////////////////////////////////////
	// ensure you can ping to nats-server, if its not found check your hosts file and server availability
	// telnet nats-server 4222 [Subscribe to App's Output Channel, NATSPORT=4222 by default but needed in telnet syntax]
	// ----> sub xal.APPNAME.out 15  [15= len("APPNAME") + 8]
	// telnet nats-server 4222 [in a new window to publish commands, Note results will be recieved on App's output Channel ]
	// ----> pub xal.APPNAME 2  [2 = length of command to be sent, e.g 2 in case of !h, 3 in case of !tc1, 7 in case of !tp30.1
	// ----> COMMAND [enter command on new line and press ENTER, See output channel for results]
	// to view help: 
	// pub xal.AppName 2    [Press Enter]
	// !h					[Press Enter]
	// Any malformed command will kick you off from session immediately
	// if u see a PING on any input or ouput channel respond with a PONG , delayed PONG or no PONG will also kick you off
	// TIP: use notepad to review your commands before sending them


	//COMMAND CHANNEL HELP
	string help =
		string("[h]elp") +
		string("\r\n[s]elect\t[a..z] where A is firstObject e.g sa") +
		string("\r\n[l]ookat\t[a..z] e.g la") +
		string("\r\n[c]amera pos [x/y/z/f(orward/back)/r(strafe-rt/lft)]%f  e.g 'cx3.0'") +
		string("\r\n[c]amMode\t[t{lookat},c{chase},f{first person},a{aircraft}] e.g cp") +
		string("\r\n[t]ransform [c?] see transform of camera") +
		string("\r\n[n]ames i.e list game objects") +
		string("\r\n[t]ransform [o?] see transform of selected object") +
		string("\r\n[t]ransform [s]cale %f") +
		string("\r\n[t]ransform move [x/y/z]%f  e.g 'tx3.0'") +
		string("\r\n[t]ransform rotate [r/t/p]%f  e.g 'tp30.1'") +
		string("\r\n[w]ireframe [t/f] 'wf: wireframe false'") +
		string("\r\n[v]erbosity [l/m/h] 'vx: verbosity low/med/high' default is medium") +
		string("\r\n[ra] report accelYZ") +
		string("\r\n[rf] report fps") +
		"";
	char verbosity_lmh = 'h';
	long counter = 0;
	int fpsx = 30;
	
	
	//THINK ON WHICH REMOTE COMMANDS ARE TO BE HANDLED AND WHY
	void preProcessRemoteCommand(string base_channel, char* r) {
		char tval[512];
		if (r[0] == 'r') //report
		{
			if (r[1] == 'a') {// fps
				//sprintf(tval, "fps(%d)", fps);
#ifndef NO_NATS
				netmsg.Post("accel:"+to_string(accel.x) + "," + to_string(accel.y) + "," + to_string(accel.z));
#endif
				//netmsg.PostSub(string("Vars"), string(tval));
			}

			if (r[1] == 'f') {// fps
				//sprintf(tval, "fps(%d)", fps);
#ifndef NO_NATS
				netmsg.Post("fps:"+to_string(fpsx));
#endif
				//netmsg.PostSub(string("Vars"), string(tval));
			}
		};

		if (r[0] == 'c') //CameMode
		{
			if (r[1] == 'l') aCamera.SetMode(Camera::CAM_MODE_LOOKAT);
			if (r[1] == 'c') aCamera.SetMode(Camera::CAM_MODE_CHASE);
			if (r[1] == 'f') aCamera.SetMode(Camera::CAM_MODE_FPS);
			if (r[1] == 'a') aCamera.SetMode(Camera::CAM_MODE_FLY);
			if (r[1] == '2') aCamera.SetMode(Camera::CAM_MODE_2D);


			Camera* cc = ((Camera*)(&aCamera));
			float val = 0.0f;
			if (r[2] >= '0' && r[2] <= '9') val = atof(r + 2);

			if (r[1] == 'x') cc->Position.x += val;
			if (r[1] == 'y') cc->Position.y += val;
			if (r[1] == 'z') cc->Position.z += val;
			if (r[1] == 'f') cc->MoveForwards(val);
			if (r[1] == 's') cc->StrafeRight(val);
		};

		if (r[0] == 'd') //dump
		{
			if (r[1] == 'v') //vertices
			{
				//char vertices[256];
				for (int i = 0; i < rm->models[selectedObject->modelId]->n_vertices; i+=3) {
					//sprintf_s(vertices, 128, "v[%d]={%.2f,%.2f,%.2f}", rm.models[selectedObject->modelId]->vertex_array[i], rm.models[selectedObject->modelId]->vertex_array[i + 1], rm.models[selectedObject->modelId]->vertex_array[i + 2]);
			//		netmsg.Post("string(vertices)");
					if (i > 64) {
			//			netmsg.Post(" Dump Stopped beyond i>64");
						break;
					}
				}
			}
		};

		if (r[0] == 's') //Select
		{
			int iSel = r[1] - 'a';//A=Select 0, B=Select 1, C=Select 2
			if (iSel != '?' - 'a') {
				if (iSel >= 0 && iSel <= nGobs) {
					iSelectedObject = iSel;
					selectedObject = gobs[iSelectedObject];
				}
			}
			string name = selectedObject->Name();
#ifndef NO_NATS
			netmsg.Post("Selected " +name);
#endif
		};

		if (r[0] == 't') //transform
		{
			//int iSel = r[1] - 'a';//A=Select 0, B=Select 1, C=Select 2
			//if (iSel >= 0 && iSel <= nGobs) iSelectedObject = iSel;
			float val = atof(r + 2);
			bool cam = (iSelectedObject==0);

			if (r[1] == 's') //Scale
			{
				selectedObject->scale = val;
				//	selectedObject->Scale = val;
			};

			if (r[1] == 'x') //Transform Move x
			{
				selectedObject->pos.x += val;
				if (cam) aCamera.Position.x = selectedObject->pos.x;
			};

			if (r[1] == 'y') //Transform Move y
			{
				selectedObject->pos.y += val;
				if (cam) aCamera.Position.y = selectedObject->pos.y;
			};

			if (r[1] == 'z') //Transform Move z
			{
				selectedObject->pos.z += val;
				if (cam) aCamera.Position.z = selectedObject->pos.z;
			};

			if (r[1] == 'r') //Transform Rot r
			{
				selectedObject->rot.x += val;
				if (cam) aCamera.RotatedX = selectedObject->rot.x;
			};

			if (r[1] == 't') //Transform Rot t
			{
				selectedObject->rot.y += val;
				if (cam) aCamera.RotatedY = selectedObject->rot.y;
			};

			if (r[1] == 'p') //Transform Rot p
			{
				selectedObject->rot.z += val;
				if (cam) aCamera.RotatedZ = selectedObject->rot.z;
			};

			if (r[1] == 'o' && r[2] == '?') //Transform Move z
			{
				sprintf(tval, "tranform(Object:%s) pos(%.1f,%.1f,%.1f) rot(%.1f,%.1f,%.1f) scale(%.1f)",
					selectedObject->Name().c_str(),
					selectedObject->pos.x, selectedObject->pos.y, selectedObject->pos.z,
					selectedObject->rot.x, selectedObject->rot.y, selectedObject->rot.z,
					selectedObject->scale
				);
#ifndef NO_NATS
               netmsg.Post(string(tval));
#endif
			};
			if (r[1] == 'c' && r[2] == '?') //Transform Move z
			{
				static char tval[200];
				f3 campos = aCamera.getPos();
				f3 camrot = aCamera.getRot();
				sprintf(tval, "PosRotScale({%.1f,%.1f,%.1f},{%.1f,%.1f,%.1f})",
					campos.x, campos.y, campos.z,
					camrot.x, camrot.x, camrot.z
				);
#ifndef NO_NATS
				netmsg.Post(string(tval));
#endif
			};

		};

		if (r[0] == 'n') //Select
		{
			string names = "gobs {";
			char c[128];
			for (int i = 1; i < nGobs; i++) {
				sprintf(c, "%c%c:%s i/%d {%d vets}\r\n", gobs[i]->hidden?'-':'+',('a' + i), gobs[i]->Name().c_str(), (int)gobs[i]->prsInstances.size(), gobs[i]->modelId>=0?rm->models[gobs[i]->modelId]->n_vertices:0);
				names += string(c);
			}
			names += "}";
#ifndef NO_NATS
			netmsg.Post(names);
#endif
		};

		//convertToPEG(p, r);//Ctransform incoming remote command to PEG::Cmd
		if (r[0] == '?' || r[0] == 'h') //Select
		{
#ifndef NO_NATS
			netmsg.Post(help);
#endif
		};

		if (r[0] == 'w') wireframe = (r[1] == 't');
		if (r[0] == 'e') edit = (r[1] == 't');

		if (r[0] == 'v') //Verbosity 
		{
			verbosity_lmh = r[1];
		};

		if (r[0] == 'p' || r[1] == 'h') //hidden status Property Hidden
		{
			static ostringstream oss;
			oss.clear();
			oss << "slected object " << gobs[iSelectedObject]->UUID	<< " is "<< ((gobs[iSelectedObject]->hidden==true)?"":"NOT") << " hidden";
#ifndef NO_NATS
			netmsg.Post(oss.str());
#endif
		};

	}

	void preProcessInput(PEG::CMD* p = NULL, float deltaT = 0.0f) {
		/*
		stringstream ss;
		int c = input.cmd[input.indx_r - 1].command;
		ss << "c";
		ss << "0x" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << c;

		ss << "r";
		ss << input.indx_r;

		ss << "w";
		ss << input.indx_w;

		if (c) wall_msg = ss.str();
		*/
		static string fmt = "%.1f,%.1f,%.1f";
		static int mX, mY;
		static bool mousepass1 = true;

		if (p->command == CMD_TOUCHMOVE || p->command == CMD_MOUSEMOVE) {
			AlgeApp::mousepos.x = p->i1;
			AlgeApp::mousepos.y = p->i2;
		}

		if (p->command == CMD_REMOTE_COMMAND) {
			preProcessRemoteCommand((char*)p->param1,((char*)p->param2)+1);
		}

		if (p->command == CMD_KEYDOWN) { aCamera.KeyDownCcam(p->i1, deltaT*300.0f); paused = false; }
		if (p->command == CMD_MOUSEWHEEL) {
			//aCamera.pos.x = aCamera.Position.x;//TBD:: merge pos and Position 
			//aCamera.pos.y = aCamera.Position.y;
			//aCamera.pos.z = aCamera.Position.y;

			aCamera.MoveForwards(p->i1 / -1.0);
		}

		///UNREAL STYLE MOUSE
		if (p->command == CMD_TOUCHMOVE || p->command == CMD_TOUCHMOVER) {
			if (p->command != CMD_TOUCH_START)
				if (mousepass1) { mX = p->i1; mY = p->i2; mousepass1 = false; return; }
			DirectionMagnitude dm = getMouseIntent(p->i1, p->i2, mX, mY);
			if (dm.directionY != ' ') {
				if (p->command == CMD_TOUCHMOVE) aCamera.MoveForwards(dm.magnitudeY);//Forwards back
				if (p->command == CMD_TOUCHMOVER) aCamera.RotateX(-dm.magnitudeY);//Ooper Neechay Dekhna
			}
			if (dm.directionX != ' ') aCamera.RotateY(-dm.magnitudeX);//left right
			mX = p->i1;	mY = p->i2;	mousepass1 = false;
		}

		if (p->command == CMD_TOUCH_END) {
			mousepass1 = true;
		}
        
        //Screen Size may have changed
        if (p->command == CMD_SCREENSIZE) {
            int retina = false;
#ifdef IOS
            retina = true;
            coord.setResolutionReported(i2(p->i1,p->i2),retina?2:1);
            ImGui_ImplAlgeSDK_Main(p->i1, p->i2, retina?2:1);
#endif

#ifdef ANDROID
	coord.setResolutionReported(i2(p->i1,p->i2),retina?2:1);
#ifndef NO_IMGUI

    ImGui_ImplAlgeSDK_Main(p->i1, p->i2, 1);
#endif

#else
    coord.setResolutionReported(i2(p->i1,p->i2),retina?2:1);
#ifndef NO_IMGUI
            ImGui_ImplAlgeSDK_Main(p->i1, p->i2, retina?2:1);
#endif
#endif
        }
        


		/////~UNREAL STYLE MOUSE
	}

	bool paused = false;

	char msg[128];
    
	class RunningAverage {
		unsigned long int counter = 0;
		long double sum = 0;
		long double r;
	public:
		void Put(float x) {
			counter++;
			sum += x;
			r = sum / counter;
		}
		float Get() {
			return float(r);
		}
	};

	RunningAverage tAvg;

    class Timeulator {
        int isec = 0;
        float fsec = 0.0;
    public:
        int curHH = 0;
        int curMM = 0;
        int curSEC = -1;

        bool tick(float dt) {
            fsec+=dt;
            int n = int(fsec);//https://stackoverflow.com/questions/3127962/c-float-to-int:
            if (isec!=n) {
                isec = n;
                return true;
            }
            return false;
        }
        
        bool tickMM(float dt) {
            if (tickSS(dt)) {
                if (curSEC==0)
                    return true;
            }
            return false;
        }
        
        bool tickSS(float dt) {
            if (tick(dt)) {
                curSEC++;
                if (curSEC>=60) {
                    curMM++;
                    curSEC=0;
                    if (curMM>=60) {
                        curHH++;
                        curMM=0;
                        if (curHH>=24) {
                            curHH=0;
                            curMM=0;
                            
                        }
                    }
                }
                return true;
            }
            return false;
        }
        
        i2 nowis ()
        {
            time_t now = time(NULL);
            struct tm *tm_struct = localtime(&now);
            return i2(tm_struct->tm_hour, tm_struct->tm_min);
        }
        
        void test() {
       //     bool b1 = tick(0.4);
       //     bool b2 = tick(0.4);
        //    bool b3 = tick(0.4);
       //     bool b4 = tick(0.4);
        }
        Timeulator() {
            i2 n = nowis();
            curHH = n.x;
            curMM = n.y;
        }
    };
    
    Timeulator timulator;

	bool draw_physics_debug = false;
    aL10 tester;
	//f3 lastAccel;

	//if overriden app is expected to return true;
	virtual bool RenderDebug(float dt) {
		return false;
	}

	void Render(float dT, int aX, int aY, int aZ) {

		//in this case everything will be done by
		if (RenderDebug(dT)) return;

		fpsx = 1.0 / dT;
		accel.x = aX/ 100.;
		accel.y = aY/100.;
		accel.z = aZ/100.;
		f3 newAccel (accel.x, accel.y, accel.z);
		
		onAcceclerometerValues(newAccel);
		//lastAccel = newAccel;

        if (timulator.tickMM(dT)) input.pushI(CMD_HHMM,timulator.curHH,timulator.curMM);
		tAvg.Put(dT);
		float deltaT = tAvg.Get() * timeMultiplier;  //Always use running average of incoming dt
		
	//	((GameObject*)&aCamera)->Update(deltaT);

		if (aCamera.GetMode() == Camera::CAM_MODE_LOOKAT) {
		//	int t = aZ;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		timeVar += deltaT;

		if (!edit) {
			
			if (!paused) {
				Update(deltaT);
				UpdatePhysics(deltaT);
				
				for (int i = 0; i < nGobs; i++) {
					GameObject* it = gobs[i];
#ifndef NO_LOGS
				//	fprintf(f, "\n%f:G#%d pos=%s", timeVar,i, it->pos.str("%f,%f,%f").c_str());
#endif
					it->Update(deltaT);
					if (i == 0) {
						aCamera.Update(deltaT, &aCamera);
					//	aCamera.OnPosRotChanged();
						
					}
					if (it->m_actionComplete) {
						it->m_actionComplete = false;
						onActionComplete(it);
					}
				}
			}
			else {
			//	bool break1 = true;
			}
		}
		//
#define DV /1.0
		if (aCamera.GetMode() == Camera::CAM_MODE_2D) {
			ViewOrthoBegin(1100); // Must End
		}

		if (aCamera.GetMode() == Camera::CAM_MODE_FPS) {
			glLoadIdentity();
			glRotatef(-aCamera.RotatedX, 1.0, 0.0, 0.0);
			glRotatef(-aCamera.RotatedY, 0.0, 1.0, 0.0);
			glRotatef(-aCamera.RotatedZ, 0.0, 0.0, 1.0);
			glTranslatef(-aCamera.Position.x, -aCamera.Position.y, -aCamera.Position.z);
		}

		if (aCamera.GetMode() == Camera::CAM_MODE_LOOKAT) {
			glLoadIdentity();
			f3 cp = aCamera.getPos();
			if (selectedObject)
			aluLookAt(cp.x, cp.y, cp.z, selectedObject->pos.x, selectedObject->pos.y, selectedObject->pos.z, 0., 1., 0.);
			aCamera.pos = cp;
		}

       

        
        //
		//processInput(p);
        
		
		renderObjects(deltaT, true);
#ifndef NO_BOX2D
		if (world && draw_physics_debug) world->DrawDebugData();
#endif
		
		if (aCamera.GetMode() == Camera::CAM_MODE_2D) ViewOrthoEnd();
	//	if (!edit) aCamera.PosRot({ aCamera.pos.x, aCamera.pos.y, aCamera.pos.z }, { aCamera.rot.x, aCamera.rot.y, aCamera.rot.z });
        
	}
    

	int LoadModel(GameObject* go, ResourceInf* res) {
		go->modelId = alLoadModel(res);
		go->resInf = res;
		go->custom_type = go->modelId;
#ifndef NO_NATS
		netmsg.Post(string("App::LoadModel:") + string(res->alx) + "," + string(res->tex));
#endif

#ifndef NO_LOGS
		//fprintf(f,"\n%s,modelID=%d,n_vertices=%d",(string("App::LoadModel:") + string(res->alx) + "," + string(res->tex)).c_str(), go->modelId, rm.models[0]->n_vertices);
#endif
		return go->modelId;
	}

	////////////////////////////PHYSICS 2D Box2D
	//Phys2D phys;
		
	int velocityIterations = 6;
	int positionIterations = 2;
#ifndef NO_BOX2D
    DbgDraw dbgDraw;
    void InitPhysics(b2Draw *dbg = 0 , float gravity = 9.8) {
#else
	void InitPhysics(void *dbg = 0 ) {
#endif
#ifndef NO_BOX2D
		world = new b2World(b2Vec2(0, gravity));
        if (dbg) {
            world->SetDebugDraw(dbg);
            dbg->SetFlags( b2Draw::e_shapeBit );
        } else {
            world->SetDebugDraw(&dbgDraw);
            dbgDraw.SetFlags( b2Draw::e_shapeBit );
            dbgDraw.Set(rightSide, bottomSide);
        }
#endif
	}

	void UpdatePhysics(float deltaT) {
#ifndef NO_BOX2D
		if (world) {
			world->Step(deltaT, velocityIterations, positionIterations);
			world->ClearForces();
		}
#endif
	}

#ifndef NO_BOX2D
	b2Body* AddFixturedBody(b2Body* body, float width=1.0, float height = 1.0) {
		// Define another box shape for our dynamic body.
		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox(width, height);
		// Define the dynamic body fixture.
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;
		// Set the box density to be non-zero, so it will be dynamic.
		fixtureDef.density = 1.0f;
		// Override the default friction.
		fixtureDef.friction = 0.3f;
		// Add the shape to the body.
		body->CreateFixture(&fixtureDef);
		return body;
	}

	b2Body* MakeDynamicBody(PosRotScale* gob_as_prs) {
		// Define the dynamic body. We set its position and call the body factory.
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(gob_as_prs->pos.x, gob_as_prs->pos.y);
		return AddFixturedBody(world->CreateBody(&bodyDef),gob_as_prs->m_width, gob_as_prs->m_height);
	}

	b2Body* MakeStaticBody(PosRotScale* gob_as_prs,float width=1.0, float height=1.0) {
		// Define the dynamic body. We set its position and call the body factory.
		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(gob_as_prs->pos.x, gob_as_prs->pos.y);
		return AddFixturedBody(world->CreateBody(&bodyDef), width, height);
	}
#endif

	int metersX, pixX;
	int metersY, pixY;
	
	void PhysicsGroundWork(float widthMeters, int _pixX, int _pixY) {
		
		metersX = width;
		metersY = metersX / _pixX * _pixY;
		pixX = _pixX;
		pixY = _pixY;
#ifndef NO_BOX2D
		// Define the gravity vector.
		b2Vec2 gravity;// (0.0f, -10.0f);
		
		int dirY = -1;
		
	//	int32 velocityIterations = 6;
	//	int32 positionIterations = 2;
		f2 ownerSize;

		gravity.x = 0.0; gravity.y = 100.0 * dirY;
		pWorld = new b2World(gravity);
		b2BodyDef groundBodyDef;
		groundBodyDef.position.Set(0.0f, -10.0f);
		// Call the body factory which allocates memory for the ground body
		// from a pool and creates the ground box shape (also from a pool).
		// The body is also added to the world.
		b2Body* groundBody = pWorld->CreateBody(&groundBodyDef);
		// Define the ground box shape.
		b2PolygonShape groundBox;
		// The extents are the half-widths of the box.
		groundBox.SetAsBox(widthMeters, 10.0f);
		// Add the ground fixture to the ground body.
		groundBody->CreateFixture(&groundBox, 0.0f);
#endif
	}
	/////////////
#ifndef NO_BOX2D
	b2PolygonShape polygon;
	b2CircleShape circle;
	b2FixtureDef bxFixDef, blFixDef;
	b2BodyDef bodyDefBall;
	b2BodyDef bodyDefBox;
#endif



	void AddMultiplePhysicalInstances(GameObject* o, int count, bool is_circle = false, float density = 1.0, float restitution = 0.1, float scale = 1.0) {
#ifndef NO_BOX2D
		#define max_generate_obj 99

		char szuuid[64];
		static PosRotScale px[max_generate_obj];
		static b2Body* px_body[max_generate_obj];
		
		f2 obj_size = f2(o->m_width, o->m_height).half();
		
		polygon.SetAsBox(obj_size.x * S2P, obj_size.y * S2P); //20 is size from alx
		circle.m_radius = obj_size.x * S2P;//20 is size from alx

		bxFixDef.shape = &polygon;
		blFixDef.shape = &circle;

		bxFixDef.density = density;
		blFixDef.density = density;

		bxFixDef.restitution = restitution;
		blFixDef.restitution = restitution;

	
	//	bxFixDef.userData = o;
	//	blFixDef.userData = o;

		bodyDefBall.type = density == -1 ? b2_staticBody :b2_dynamicBody;
		bodyDefBox.type  = density == -1 ? b2_staticBody : b2_dynamicBody;

		if (count > max_generate_obj) {
			output.pushP(CMD_TOAST, $ "numInstances exceeds limit", 0);
			return;
		}

		for (int i = 0; i < count; i++) {

			float rndScale = 1.0;// 0.5 + randm();
			float box_hh = obj_size.x  * rndScale;
			float box_hw = obj_size.y * rndScale;

			polygon.SetAsBox(box_hh * S2P, box_hw * S2P);
			//if (i==0) bodyDefBox.position.Set(0.5 * 7, 0.5 * 5);//just to test
			//else 
			bodyDefBox.position.Set(randm() * rightSide * S2P, randm() * -20);
            
			px_body[i] = (world) ? world->CreateBody(&bodyDefBox) : 0;
			b2FixtureDef* fixDef = (is_circle?&blFixDef:&bxFixDef);
			if (world) {
				if (px_body[i]!=0) {
					
					b2Fixture* f = px_body[i]->CreateFixture(fixDef);
					f->SetUserData(o);
					px[i].physBodyPtr = px_body[i];
				}
			}
			px[i].scale = rndScale;//?
			px[i].pos.x = bodyDefBox.position.x * P2S;
			px[i].pos.y = bodyDefBox.position.y * P2S;
			px[i].m_height = 2 * box_hh;
			px[i].m_width = 2 * box_hw; // == ball_rad = 20 in this case

			//bm[i].userId = i;
			sprintf(szuuid, "%s#%d", o->UUID.c_str(), i);
			px[i].UUID = string(szuuid);
			o->AddInstance(px[i], i);
			
		//	o->instanceNo = (i);
		}
#endif
	}

#ifndef NO_BOX2D
	b2BodyDef bodyDefWalls;
	b2PolygonShape shp;
#endif

    void PhysAddGroundWithWalls(int height=1.0) {
#ifndef NO_BOX2D
		bxFixDef.shape = &shp;
		// create ground
        float half_width_of_box = rightSide * S2P / 2; float half_height_of_box = height * S2P;
        float position_of_box_x = rightSide * S2P / 2; float position_of_box_y = bottomSide * S2P;
        
		//half_width_of_box = 500 * S2P / 2;  half_height_of_box = 10 * S2P;
		//position_of_box_x = 500 * S2P / 2;  position_of_box_y = 500 * S2P;

        shp.SetAsBox(half_width_of_box,half_height_of_box);
        bodyDefWalls.position.Set(position_of_box_x,position_of_box_y);
        
		if (world) world->CreateBody(&bodyDefWalls)->CreateFixture(&bxFixDef);
        
        half_width_of_box = 1 * S2P; half_height_of_box = bottomSide * S2P;
        shp.SetAsBox(half_width_of_box,half_height_of_box);
		
		// left wall
		bodyDefWalls.position.Set(0, bottomSide * S2P / 2);
		if (world) world->CreateBody(&bodyDefWalls)->CreateFixture(&bxFixDef);
		// right wall
		bodyDefWalls.position.Set(rightSide * S2P, bottomSide * S2P / 2);
		if (world) world->CreateBody(&bodyDefWalls)->CreateFixture(&bxFixDef);
#endif
	}

	void PhysAddBoxedArea(int left,int top, int bottom, int right) {
#ifndef NO_BOX2D
		bxFixDef.shape = &shp;
		// create ground
		float half_width_of_box = rightSide * S2P / 2; float half_height_of_box = height * S2P;
		float position_of_box_x = rightSide * S2P / 2; float position_of_box_y = bottomSide * S2P;

		//half_width_of_box = 500 * S2P / 2;  half_height_of_box = 10 * S2P;
		//position_of_box_x = 500 * S2P / 2;  position_of_box_y = 500 * S2P;

		shp.SetAsBox(half_width_of_box, half_height_of_box);
		bodyDefWalls.position.Set(position_of_box_x, position_of_box_y);

		if (world) world->CreateBody(&bodyDefWalls)->CreateFixture(&bxFixDef);

		half_width_of_box = 1 * S2P; half_height_of_box = bottomSide * S2P;
		shp.SetAsBox(half_width_of_box, half_height_of_box);

		// left wall
		bodyDefWalls.position.Set(left, bottom * S2P );
		if (world) world->CreateBody(&bodyDefWalls)->CreateFixture(&bxFixDef);
		// right wall
		bodyDefWalls.position.Set(right * S2P, bottom * S2P );
		if (world) world->CreateBody(&bodyDefWalls)->CreateFixture(&bxFixDef);
		// top wall
		bodyDefWalls.position.Set(left * S2P, bottom * S2P);
		if (world) world->CreateBody(&bodyDefWalls)->CreateFixture(&bxFixDef);
		// right wall
		bodyDefWalls.position.Set(right * S2P, bottom * S2P);
		if (world) world->CreateBody(&bodyDefWalls)->CreateFixture(&bxFixDef);
#endif
	}

	//https://stackoverflow.com/questions/401847/circle-rectangle-collision-detection-intersection
	bool isCircleIntersectingRect(float circleX, float circleY, float circleRadius, float rectX, float rectY, float rectWidth, float rectHeight)
	{
		f2 circleDistance;
		circleDistance.x = abs(circleX - rectX);
		circleDistance.y = abs(circleY - rectY);

		if (circleDistance.x > (rectWidth / 2 + circleRadius)) { return false; }
		if (circleDistance.y > (rectHeight / 2 + circleRadius)) { return false; }

		if (circleDistance.x <= (rectWidth / 2)) { return true; }
		if (circleDistance.y <= (rectHeight / 2)) { return true; }

		float cornerDistance_sq = (circleDistance.x - rectWidth / 2)*(circleDistance.x - rectWidth / 2)
			+ (circleDistance.y - rectHeight / 2) * (circleDistance.y - rectHeight / 2);

		return (cornerDistance_sq <= (circleRadius*circleRadius));
	}

	static string snd_names[16];

	static int snd_counter;

	const int AddSound(const char* soundfile) {
		int sid = snd_counter;
		output.pushP(sid, (void*)soundfile);
		snd_names[sid] = string(soundfile);
		snd_counter++;
		return sid;
	}

	string collid;

#ifndef NO_BOX2D

	void BeginContact(b2Contact* contact) {
		GameObject* ob1 = static_cast<GameObject*>(contact->GetFixtureA()->GetUserData());
		GameObject* ob2 = static_cast<GameObject*>(contact->GetFixtureB()->GetUserData());

		if (ob1 && ob2) onContact(ob1->Name() + "+" + ob2->Name());
		
	}

	void EndContact(b2Contact* contact) {
		GameObject* ob1 = static_cast<GameObject*>(contact->GetFixtureA()->GetUserData());
		GameObject* ob2 = static_cast<GameObject*>(contact->GetFixtureB()->GetUserData());

		if (ob1 && ob2) onContactEnd(ob1->Name() + "+" + ob2->Name());
	}

#endif // !NO_BOX2D

	virtual void onContact(string collid) {}

	virtual void onContactEnd(string collid) {}

	void PlaySnd(int idx = 0) {
		output.pushP(CMD_SNDSET0 + idx, $ snd_names[idx].c_str(), 0);
		output.pushP(CMD_SNDPLAY0 + idx, $ snd_names[idx].c_str(), 0);
	}

	void PlaySndEx(char * name, int idx = 0) {
		output.pushP(CMD_SNDSET0+idx, $ name, 0);
		output.pushP(CMD_SNDPLAY0+idx, $ name, 0);
	}

	AlgeApp* SetName(const char* _name) {
		name = string(_name);
		return this;
	}

	void SetTitle(const char * name) {
	    static string n;
		n = string(name);
		output.pushP(CMD_TITLE, $ n.c_str(), 0);
		return;
	}


};

int AlgeApp::snd_counter = 0;
string AlgeApp::snd_names[16];
string AlgeApp::touched_bodies_list = "";
string AlgeApp::clickedbody = "";
AlgeApp* AlgeApp::thiz = nullptr;
f3 AlgeApp::mousepos;


CResourceManager* AlgeApp::rm;
CResourceManager AlgeApp::m_rm;

class XWidget {
protected:
	AlgeApp* that;
public:
	virtual void setPos(f3 pos) {}
};

class XButton : public XWidget {

	bool pressed;
	GameObject gob_pressed, gob_unpressed, gob_hovered;


public:
	f3 MyPos;
	GameObject* ref;
	string tag;

	void setPress(bool isPressed) {

		gob_pressed.hidden = true;
		gob_unpressed.hidden = false;
		gob_hovered.hidden = true;

		if (isHovered) {
			gob_pressed.hidden = true;
			gob_unpressed.hidden = true;
			gob_hovered.hidden = false;
		}

		if (isPressed) {
			gob_pressed.hidden = false;
			gob_unpressed.hidden = true;
			gob_hovered.hidden = true;
		}

	}

	bool isYours(GameObject* gob) {
		bool t1 = gob->is(gob_pressed);
		bool t2 = gob->is(gob_unpressed);
		bool t3 = gob->is(gob_hovered);
		bool t = t1 || t2 || t3;
		return t;
	}

	void setPos(f3 p) {
		gob_pressed.SetPos(p);
		gob_unpressed.SetPos(p);// (f3(p.x, p.y + 100, p.z));
		gob_hovered.SetPos(p);// f3(p.x, p.y + 200, p.z));
	}

	void LoadIn(AlgeApp* thiz, string tag, bool tga=false) {
		that = thiz;
		string prefix =  tag + "/btn_" + tag + "_";
		if (prefix.find_first_of(".btn") != string::npos) {
			prefix =   tag + ".btn/btn_" + tag + "_";		
		}
		std::replace(prefix.begin(), prefix.end(), ' ', '_');

		if (!tga) {//assume jpg if not tga
			ref = thiz->AddResource(&gob_unpressed, prefix + "U", prefix + "U.jpg");//Unpressed
			thiz->AddResource(&gob_pressed, prefix + "P", prefix + "P.jpg");//Pressed
			thiz->AddResource(&gob_hovered, prefix + "H", prefix + "H.jpg");//Hover
		} else {
			ref = thiz->AddResource(&gob_unpressed, prefix + "U");//Unpressed
			thiz->AddResource(&gob_pressed, prefix + "P");//Pressed
			thiz->AddResource(&gob_hovered, prefix + "H");//Hover
		}

		setPress(false);
		screenCenter.x = (thiz->rightSide / 2);
		screenCenter.y = (thiz->bottomSide / 2);
		toOrigin();
	}

	void LoadInEx(AlgeApp* thiz, string prefix) {
		that = thiz;
		ref = thiz->AddResource(&gob_unpressed, prefix + "0");
		thiz->AddResource(&gob_pressed, prefix + "1");
		thiz->AddResource(&gob_hovered, prefix + "2");
		setPress(false);
		screenCenter.x = (thiz->rightSide / 2);
		screenCenter.y = (thiz->bottomSide / 2);
		toOrigin();
	}

	void toOrigin() {
		MyPos.x = screenCenter.x;
		MyPos.y = screenCenter.y;
	}

	f3 screenCenter;

	bool isHovered;
	bool isTouched;

	bool fired = false;

	void processInput(PEG::CMD* p, float deltaT) {
		//	isHovered = false;

		if (p->command == CMD_MOUSEMOVE) {
	//		isHovered = (that->doPicking2D(ref, AlgeApp::mousepos.xy()));
		}

		if (p->command == CMD_TOUCH_START) {
			isTouched = true;
		}
		else if (p->command == CMD_TOUCH_END) {
			isTouched = false;
		}


		if (isHovered) {
			// if hovering unhide gob_hovered
			gob_hovered.hidden = false;

			if (isTouched) {
				gob_hovered.hidden = true;
				gob_pressed.hidden = true;
				gob_unpressed.hidden = false;
				if (!fired) {
					that->onWidgetEvent((GameObject*)this->ref, p);
					fired = true;
				}
			}
			else {
				gob_hovered.hidden = false;
				gob_pressed.hidden = true;
				gob_unpressed.hidden = true;
				fired = false;
			}

		}
		else {
			//else hide 
			gob_hovered.hidden = true;

			{
				gob_hovered.hidden = true;
				gob_pressed.hidden = true;
				gob_unpressed.hidden = false;//show
			}

		}





	}

	void Update(float dt, AlgeApp* app) {

	}
};


class XFlipFlopSprite {

    float heading; //0 to 180
    bool hidden;
public:
    f3 MyPos;
    GameObject flip, flop;

    void hide(bool b) {
        if (!b) flip.hidden = b;
        if (b) flop.hidden = b;
    }

    void avoidOrigin() {
        while (abs(MyPos.x - screenCenter.x) < 200) MyPos.x += 1;
        while (abs(MyPos.y - screenCenter.y) < 200) MyPos.y += 1;
    }
    void toOrigin() {
        MyPos.x = screenCenter.x;
        MyPos.y = screenCenter.y;
    }

    f3 screenCenter;

    void LoadIn(AlgeApp* thiz, string prefix) {
        thiz->AddResource(&flip, prefix + "1");
        thiz->AddResource(&flop, prefix + "2");
        flip.hidden = false;
        flop.hidden = false;
        heading = rndm(100, 360);
        MyPos.x = rndm(thiz->leftSide, thiz->rightSide);
        MyPos.y = rndm(thiz->topSide, thiz->bottomSide);
        screenCenter.x = (thiz->rightSide /2);
        screenCenter.y = (thiz->bottomSide /2);
    }

    bool toggle;
    float timeVar;

    bool IsItYou(GameObject* g) {
        return ( g == &flip ||  g == &flop );
    }

    void Update(float dt, AlgeApp* app) {
        timeVar += dt;
        if (timeVar > 0.5) {
            timeVar = 0.0;
            toggle = !toggle;
            if (toggle) {
                flip.hidden = true;
                flop.hidden = false;
            } else 	{
                flip.hidden = false;
                flop.hidden = true;
            }
        }

        float dy = 100 * dt * sin(heading / FACTOR_RADIANS_DEGREES);
        float dx = 100 * dt * cos(heading / FACTOR_RADIANS_DEGREES);
        MyPos.x += dx;
        MyPos.y += dy;
        if (MyPos.x < app->leftSide) MyPos.x = app->rightSide;
        if (MyPos.x > app->rightSide) MyPos.x = app->leftSide;
        if (MyPos.y < app->topSide) MyPos.y = app->bottomSide;
        if (MyPos.y > app->bottomSide) MyPos.y = app->topSide;
        flip.pos = MyPos;
        flop.pos = MyPos;
        flip.rot.z = heading;
        flop.rot.z = heading;
    }
};

class XWalkingSprite {
    f3 MyPos;
    char UDLR = ' ';
public:
    GameObject playerGrey_up1, playerGrey_up2, playerGrey_walk1, playerGrey_walk2;

    void SetIntent(char dir) {
        UDLR = dir;
    }

    void toOrigin(GameObject& g) {
        g.pos.x = (screenRect.Left-screenRect.Right)/2;
        g.pos.y = (screenRect.Bottom-screenRect.Right)/2;
    }

    void toOrigin() {
        toOrigin(playerGrey_up1);
        toOrigin(playerGrey_up2);
        toOrigin(playerGrey_walk1);
        toOrigin(playerGrey_walk2);
    }

    bool IsItYou(GameObject* g) {
        return (g == &playerGrey_up1 ||	g == &playerGrey_up2 || g == &playerGrey_walk1 || g == &playerGrey_walk2 );
    }

    void SetVisibility() {
        playerGrey_up1.hidden = true;
        playerGrey_up2.hidden = true;
        playerGrey_walk1.hidden = true;
        playerGrey_walk2.hidden = true;

        if ((UDLR == 'U' || UDLR == ' ') &&  toggle) {
            playerGrey_up1.hidden = false; playerGrey_up1.rot.z = 0;
            return;
        }
        if ((UDLR == 'U' || UDLR == 'C' || UDLR == ' ') && !toggle) {
            playerGrey_up2.hidden = false; playerGrey_up2.rot.z = 0;
            return;
        }
        if (UDLR == 'L' && toggle) {
            playerGrey_walk1.rot.y = 180;
            playerGrey_walk1.hidden = false;
            return;
        }
        if (UDLR == 'L' && !toggle) {
            playerGrey_walk2.rot.y = 180;
            playerGrey_walk2.hidden = false;
            return;
        }
        if (UDLR == 'R' && toggle) {
            playerGrey_walk1.rot.y = 0;
            playerGrey_walk1.hidden = false;
            return;
        }

        if (UDLR == 'R' && !toggle) {
            playerGrey_walk2.rot.y = 0;
            playerGrey_walk2.hidden = false;
            return;
        }

        if (UDLR == 'D' && toggle) {
            playerGrey_up1.hidden = false; playerGrey_up1.rot.z = 180;
            return;
        }
        if (UDLR == 'D' && !toggle) {
            playerGrey_up2.hidden = false; playerGrey_up2.rot.z = 180;
            return;
        }
        if (UDLR == ' ' && !toggle) {
            playerGrey_up2.hidden = false; playerGrey_up2.rot.z = 180;
            return;
        }


    }

    bool toggle;
    float timeVar = 0;

    AlgeApp* that;

    void Update(float dt, AlgeApp* app) {
        timeVar += dt;
        that = app;
        const int dd = 10;

        if (UDLR == 'U')
            SetPos(f3(MyPos.x,		MyPos.y>0?MyPos.y-dd:MyPos.y,					MyPos.z));
        if (UDLR == 'D')
            SetPos(f3(MyPos.x,		MyPos.y<app->bottomSide?MyPos.y+dd:MyPos.y,		MyPos.z));
        if (UDLR == 'L')
            SetPos(f3(MyPos.x>0?MyPos.x-dd:MyPos.x,					MyPos.y, MyPos.z));
        if (UDLR == 'R')
            SetPos(f3(MyPos.x<app->rightSide?MyPos.x+dd:MyPos.x,	MyPos.y, MyPos.z));

        if (timeVar >= 1.0) {
            timeVar = 0.0;
            toggle = !toggle;
        }

        SetVisibility();
    }

    void SetPos(f3 pos) {
        MyPos.x = pos.x;
        MyPos.y = pos.y;
        MyPos.z = pos.z;
        playerGrey_up1.pos = MyPos;
        playerGrey_up2.pos = MyPos;
        playerGrey_walk1.pos = MyPos;
        playerGrey_walk2.pos = MyPos;
    }

    f3 GetPos() { return MyPos; }
	

    void WasHit() {
        playerGrey_up1.JuiceType = JuiceTypes::JUICE_SCALE_IN;
        UDLR = ' ';
    }

    float playerGrey_up1_width = 10.;

    float GetWidth() { return playerGrey_up1_width; }

    void LoadIn(AlgeApp* thiz, string tagUpward, string tagRightward) {
        thiz->AddResource(&playerGrey_up1, tagUpward+"1");
        thiz->AddResource(&playerGrey_up2, tagUpward+"2");
        thiz->AddResource(&playerGrey_walk1, tagRightward+"1");
        thiz->AddResource(&playerGrey_walk2, tagRightward+"2");
        SetPos(f3(thiz->rightSide / 2, thiz->bottomSide / 2, 0));
        playerGrey_up1.pos = MyPos;
        playerGrey_up1_width = playerGrey_up1.getOwnRect().Right - playerGrey_up1.getOwnRect().Left;
    }
};
class GameScene {
    
};

class GameObjectSnd {
public:
    GameObject obj;
    string tag;// filenames derived from tag
    
    void AddResourceWithSound(AlgeApp* app, string alx_tag, string tga_tag, float scale = 1.0) {
        obj.JuiceType = 0;
        app->AddResource(&obj, alx_tag, tga_tag, scale);
		auto dimen = obj.GetDim();
        app->AddSound((tag+".wav").c_str());
    }
};
class FontObject : public GameObject {
public:
    
};


class DPad : public GameObject {
	
public:
	bool enabled;
    AlgeApp* that;
    
	string m_tag;
	DPad() {};

	DPad(string filetag) {
		m_tag = filetag;
		enabled = true;
	}

	GameObject* LoadIn(AlgeApp* thiz) {
        that = thiz;
		if (m_tag.size() == 0) m_tag = "dpad";
		GameObject* d = thiz->AddResource(this, m_tag, m_tag, thiz->screenScale);
		pos.x = thiz->rightSide - 64;
		pos.y = thiz->bottomSide / 2;
		JuiceType = 0;// JuiceTypes::JUICE_SCALE_IN;
		JuiceSpeed *= 2;
		color = f3(0.9, 0.9, 0.9);
		center = pos;
		hud = true;
		return d;
	}

	f3 center;

	virtual void Update(float deltaT) {

		if (wasTouched()) {
	//		int t = UDLRC();
		}
		else {
       
			rot.x = 0;
			rot.y = 0;
			rot.z = 0;
		}
	}

	//udlr = updownleftrt
	char UDLRC() {
		f2 p = posTouched();
		f2 pt = f2(p.x - center.x, center.y - p.y);
		int row, col = 0;
		int a = this->m_width / 6;
		if (pt.x <= -a) col = 0; else
			if (pt.x > -a && pt.x < a) col = 1; else
				if (pt.x >= a) col = 2;

		if (pt.y >= a) row = 0; else
			if (pt.y > -a && pt.y < a) row = 1; else
				if (pt.y <= a) row = 2;

		char ret = ' ';
		if (col == 0) {
			if (row == 1) ret = 'L';
		}
		if (col == 1) {
			if (row == 0) ret = 'U';
			if (row == 1) ret = 'C';
			if (row == 2) ret = 'D';
		}
		if (col == 2) {
			if (row == 1) ret = 'R';
		}

	//	Swivel(ret);

		return ret;
	}

	void Swivel(char dir) {
		rot.z = 0;
		if (dir == 'U') rot.x = -20;
		if (dir == 'D') rot.x = +20;
		if (dir == 'L') rot.y = -20;
		if (dir == 'R') rot.y = +20;
		if (dir == 'C' || dir == ' ') {
			rot.x = 0; rot.y = 0;
		}
	}


};

class DBtn : public GameObject {
	string m_tag;

public:

	DBtn() {};

	DBtn(string filetag) {
		m_tag = filetag;
	}

	GameObject* LoadIn(AlgeApp* thiz, string m_tag, float scale = 1.0) {
		if (m_tag.size() == 0) m_tag = "pointer";
		GameObject* d = thiz->AddResource(this, m_tag, m_tag, scale);
		pos.x = thiz->rightSide - 64;
		pos.y = thiz->topSide + 64;
		JuiceType = JuiceTypes::JUICE_SCALE_IN;
		JuiceSpeed *= 2;
		color = f3(0.9, 0.9, 0.9);
		hud = true;
		return d;
	}
	
	void SetDirection(int angle) {
		rot.z = angle;//??
	//	rot.x = degrees;//??
	//	rot.y = degrees;//??
	}
	
};


// specs:
// i1=='p' AND i2= 'U' : Pad Up //'D' : Pad Dn // 'L' : Pad Up //'R' : Pad Dn 
// i1=='b' AND i2 = buttonID
class MyGamePad {
public:
	enum EventTypes { PAD = 'p', BTN = 'b' };
	enum EventCodes { PAD_UP = 'U', PAD_DN = 'D', PAD_LT = 'L', PAD_RT = 'R' , BTN_DELTA=0, BTN_SQUARE=3, BTN_X=2, BTN_CIRCLE=1 };

	static char get_i1(unsigned int buttonID, int axisID, float value, void* context) {
		static char i1 = '?';
		if (buttonID == -1) {//axis
			i1 = EventTypes::PAD;
		}
		else {//button
			i1 = EventTypes::BTN;
		}
		return i1;
	}
	
	static char get_i2(unsigned int buttonID, int axisID, float value, void* context) {
		static char i2 = '?';
		if (buttonID == -1) {//axis
			if (axisID == 5 && value == -1.0) i2 = PAD_UP;
			if (axisID == 5 && value == 1.0) i2 = PAD_DN;
			if (axisID == 4 && value == 1.0) i2 = PAD_RT;
			if (axisID == 4 && value == -1.0) i2 = PAD_LT;
		}
		else {//button
			i2 = buttonID;
		}
		return i2;
	}
};

//added 04 Feb 2014
class SettingScreen : public GameObject {
public:

	enum EventNames {
		Exiting = 1
	};

	DBtn p1, p2, p3;
	DBtn ico;

	string m_tag;
	float x[3] = {0,0,0};
	DBtn *p;

	SettingScreen() {};
	int ix[3] = { 0,0,0 };

	void RepositionObject(int rightSide, int bottomSide) {
		int w = ico.getOwnRect().Right - ico.getOwnRect().Left;
		int h = ico.getOwnRect().Bottom - ico.getOwnRect().Top;
		ico.pos.x = 0.95*rightSide;
		ico.pos.y = 0.1*bottomSide;
	}
	int getIdFromX(int x) {
		if (abs(x - 1056) < 50) return 2;
		if (abs(x - 818) < 50) return 1;
		if (abs(x - 581) < 50) return 0;
		return -1;
	}
	/*
	int getSettingOfOrientation(int posx) {
		ix[1] = getIdFromX(posx);
		return ix[1] == 0 ? 0 : 1;
	}

	int getSettingOfDifficulty(int posx) {
		ix[2] = getIdFromX(posx); 
		return ix[2];
	}

	int getSettingOfControlMode(int posx) {
		ix[0] = getIdFromX(posx); 
		return ix[0];
	}
	*/
	SettingScreen(string filetag) {
		m_tag = filetag;
		valueControlMethod = 1;// Touch
		valueDifficulty = 0;//Easy
		valueOrientation = 0;//landscape
	}
	f3 center;
	
	void RollDown() { 
		p->JuiceType = 0;
		if (p == &p2) p = &p3;
		if (p == &p1) p = &p2;
		p->JuiceType = JUICE_PULSATE_FULLY;
	}
	void RollUp() {
		p->JuiceType = 0;
		if (p == &p2) p = &p1;
		if (p == &p3) p = &p2;
		p->JuiceType = JUICE_PULSATE_FULLY;
	}

	int Which() {
		int which = 0;
		if (p1.JuiceType == JuiceTypes::JUICE_PULSATE_FULLY) which = 1;
		if (p2.JuiceType == JuiceTypes::JUICE_PULSATE_FULLY) which = 2;
		if (p3.JuiceType == JuiceTypes::JUICE_PULSATE_FULLY) which = 3;
		return which;
	}

	void NotifyApp(int ptrId, int posx) {
		
		switch (ptrId) {
		case 0:
			valueControlMethod = getIdFromX(posx);
			break;
		case 1:
			valueOrientation = getIdFromX(posx);
			break;
		case 2:
			valueDifficulty = getIdFromX(posx);
			break;
		default:
			break;
		}
	}



	void RollLeft() {
		int ip = Which() - 1;
		ix[ip]--;
		if (ix[ip] < 0) ix[ip] = 0;
		p->pos = f3(anchors_v1[ip][ix[ip]].x, anchors_v1[ip][ix[ip]].y, 0.0);
		NotifyApp(ip, p->pos.x);
	}

	void RollRight() {
		int ip = Which() - 1;
		ix[ip]++;
		if (ix[ip] > 2) ix[ip] = 2;
		p->pos = f3(anchors_v1[ip][ix[ip]].x, anchors_v1[ip][ix[ip]].y, 0.0);
		NotifyApp(ip, p->pos.x);
	}

	void processInput(int command, i2 loc) {
		if (p->hidden) return;

		if (command == CMD_GAMEPAD_EVENT) {
			processGamePadEvent(loc.x,loc.y);
		}
		if (command == CMD_TOUCH_START) {
			
			string ret = processTouchEvent(loc);
            printf("%s",ret.c_str());
		}
	}

	void processGamePadEvent(char type, char code) {
		if (type==MyGamePad::EventTypes::PAD) {
			if (code == MyGamePad::EventCodes::PAD_UP) RollUp();
			if (code == MyGamePad::EventCodes::PAD_DN) RollDown();
			if (code == MyGamePad::EventCodes::PAD_LT) RollLeft();
			if (code == MyGamePad::EventCodes::PAD_RT) RollRight();
		}
	}
		

	string processTouchEvent(i2 point) {
		ostringstream oss;

		if (m_thiz->onTouched("settings_icon")) {
			//	SetVisible(true,backgr);
				oss << "settings_icon;";
				m_thiz->input.pushI(CMD_SETTINGS_SCREEN, 1, 1);
				return oss.str();
		}
		
		
		CRect btnExit;
		btnExit.Right = (0.1875 * m_thiz->rightSide);
		btnExit.Top = (0.8 * m_thiz->bottomSide);
		
		if (point.x < btnExit.Right && point.y> btnExit.Top) {
			this->JuiceType = JuiceTypes::JUICE_FLY_OUT;
			p1.Hide();
			p2.Hide();
			p3.Hide();
			ico.Show();
			valueControlMethod = getIdFromX(p1.pos.x);
			valueOrientation = getIdFromX(p2.pos.x);
			valueDifficulty = getIdFromX(p3.pos.x);
			lastPointerX[0] = p1.pos.x;
			lastPointerX[1] = p2.pos.x;
			lastPointerX[2] = p3.pos.x;
			oss << "settings_exit[ controlmode=" << valueControlMethod << ", orientation=" << valueOrientation << ", difficulty=" << valueDifficulty << "];";
			m_thiz->UnDim();
			m_thiz->onSettingChanged("controlmode", valueControlMethod);
			m_thiz->onSettingChanged("orientation", valueOrientation);
			m_thiz->onSettingChanged("difficulty", valueDifficulty);
			return oss.str();
		}
				
		//find the nearest anchor
		int Yoffset = (anchors_v1[1][0].y - anchors_v1[0][0].y) / 2;
		i2 b(point.x,point.y + Yoffset);//Buttons are Yoffset above pointers
		
		bool hit(false);
		int im =0, jm=0, dmin = 1E3;
		for (int i=0; i<3; i++) for (int j = 0; j < 3; j++) {
			i2 a = anchors_v1[i][j];
			//float pt2[2];
			//pt2[0] = float(a.x);
			//pt2[1] = float(a.y);
			float dist = sqrt(pow(a.x - b.x, 2.0) + pow(a.y - b.y, 2.0));// sqrt(1234); CAnimator::Dist(pt1, pt2);
			oss << dist << "/" << dmin << ";";
			if (dist < dmin) {
				dmin = dist; im = i; jm = j; hit = true;
			}
		}
	//	oss << "im:" << im << " jm:" << jm << " hit:" << hit;
		//move to achor and select it
		f3 pt(anchors_v1[im][jm]);
	//	oss << " pt:"<< pt.str("%.1f,%.1f");
		p->JuiceType = 0;
		if (p1.pos.y == pt.y) { p1.pos = pt; p = &p1; NotifyApp(0, p1.pos.x);}
		if (p2.pos.y == pt.y) { p2.pos = pt; p = &p2; NotifyApp(1, p2.pos.x);}
		if (p3.pos.y == pt.y) { p3.pos = pt; p = &p3; NotifyApp(2, p3.pos.x);}
		p->JuiceType = JuiceTypes::JUICE_PULSATE_FULLY;



		return oss.str();
	}

//	#include "../../Apps/SettingsScreen.Assets/Data/settings.anchors"
	//anchorpoints for settings.alx & settings.tga screen
	i2 anch_size = i2(1024,512);
	i2 anchors_v1[3][3] = {
        {i2(465,185),i2(655,185),i2(845,185)},
        {i2(465,315),i2(655,315),i2(845,315)},
        {i2(465,460),i2(655,460),i2(845,460)},
	};//row col

	short valueOrientation = 0, valueDifficulty = 0, valueControlMethod = 1;
	int lastPointerX[3];

	void setXY(int col) {
		p1.pos.x = anchors_v1[0][col].x;
		p2.pos.x = anchors_v1[1][col].x;
		p3.pos.x = anchors_v1[2][col].x;
		p1.pos.y = anchors_v1[0][col].y;// thiz->bottomSide * 2 / 6;
		p2.pos.y = anchors_v1[1][col].y;// thiz->bottomSide * 4 / 6;
		p3.pos.y = anchors_v1[2][col].y;// thiz->bottomSide * 5 / 6;
	}

	void resize2Dmodel(CModel* m, i2 now) {
		for (int i = 0; i < m->n_vertices * 3; i += 3) {
			f3 vert = f3(m->vertex_array[i] / float(anch_size.x) * float(now.x), m->vertex_array[i + 1] / float(anch_size.y) * float(now.y), m->vertex_array[i + 2]);
			m->vertex_array[i] =  vert.x ;
			m->vertex_array[i + 1] = vert.y ;
		}
	}
	AlgeApp* m_thiz;
	GameObject* backgr;
	bool m_visible;

	void SetVisible(bool showit, GameObject* background = nullptr) {
		m_visible = showit;
		if (showit) m_thiz->Dim(); else m_thiz->UnDim();

		if (showit == true) {

			//ico.Hide();
			setXY(0);
			//	GameObject::Show();

			p1.JuiceType = JuiceTypes::JUICE_PULSATE_FULLY;
			p1.JuiceSpeed /= 2; p2.JuiceSpeed /= 2; p3.JuiceSpeed /= 2;
			p2.JuiceType = 0;
			p3.JuiceType = 0;
			this->scale = 0; this->JuiceType = JuiceTypes::JUICE_SCALE_IN;
			this->juice_sine_angle = 0; //resetPrev Juice Effect 
			p1.scale = 0.8;
			p2.scale = p1.scale;
			p3.scale = p1.scale;
			p1.Show();
			p2.Show();
			p3.Show();

			p1.pos.x = lastPointerX[0];
			p2.pos.x = lastPointerX[1];
			p3.pos.x = lastPointerX[2];
			
			Show();
		} 	else {
			ico.Show();
			this->JuiceType = 0;
			HideFor(&ico);
			p1.Hide();
			p2.Hide();
			p3.Hide();
		}
		
	}

	GameObject* LoadIn(AlgeApp* thiz, string tagSettings, string tagPointer, string tagIcon, GameObject* background = nullptr, bool showit = true) {
		m_thiz = thiz;
		i2 now = thiz->coord.getResolutionForGameEngine();
		//rescale anchordata as per our new screensize
		for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) {
			float xo = anchors_v1[i][j].x;
			float yo = anchors_v1[i][j].y;
			float xn = (xo / anch_size.x)*now.x;
			float yn = (yo / anch_size.y)*now.y;
			anchors_v1[i][j].x = int(xn);
			anchors_v1[i][j].y = int(yn); 
		}

		lastPointerX[0] = anchors_v1[0][valueControlMethod].x;
		lastPointerX[1] = anchors_v1[1][valueOrientation].x;
		lastPointerX[2] = anchors_v1[2][valueDifficulty].x;
		
		if (m_tag.size() == 0) m_tag = tagSettings;
        GameObject* d;
		//LOAD SETTING SCREEN
        d = thiz->AddResource(this, m_tag, XFunction_AutoScalingToFullScreen::AUTO_SCALING_FULLSCREEN);
        
	////	resize2Dmodel(thiz->rm.models[d->modelId], thiz->getBackgroundSize());
        SetVisible(showit, background);//hidden
        
		center = pos;
		//hud = true;
		p1.LoadIn(thiz, tagPointer);
		p2.LoadIn(thiz, tagPointer);
		p3.LoadIn(thiz, tagPointer);
		p = &p1;
		ico.LoadIn(thiz, tagIcon, 0.5);//LOAD SETTING ICON
		ico.JuiceType = JuiceTypes::JUICE_ROTZ;
		ico.JuiceSpeed /= 3;
		
		return d;
	}

};


class StartScreen {
public:
	GameObject bg;
	GameObject start;

	GameObject ratings;

	GameObject* LoadIn(AlgeApp* thiz, string bgd = "StartScreen") {
        
		GameObject* background = thiz->AddResource(&bg, bgd, XFunction_AutoScalingToFullScreen::AUTO_SCALING_FULLSCREEN);
        
		int middleX = thiz->rightSide / 2;

		with thiz->AddResource(&ratings, "ratings", 1.0);
		 _.pos.x = middleX;
		 _.pos.y = 0.1 * thiz->bottomSide;
		_with
     
        with thiz->AddResource(&start, "start", 1.0);
		_.pos.x = middleX;
        _.pos.y =  0.55 * thiz->bottomSide;
         _.JuiceType = JuiceTypes::JUICE_PULSATE;
        _with
		return background;
	}
	
	void SetVisible(bool visible = true) {
		if (visible) {
			ratings.Show();
			start.Show();
			bg.Show(); 
		} else {
			ratings.Hide();
			start.Hide();
			bg.Hide();
		}
	}
	

};


class MockUpOne {
public:
    GameObject score, titl;
    
    SettingScreen settings;
    StartScreen startScreen;
    DPad dPad;
    AlgeApp* app;
    
	GameObject* titleObject;

	///
	//A mock is a set of Screen and associated interactions
	//e.g a standard setting screen which shows
	// a start screen with a title and ratings image and a Start button and a settings icon on top right
	// setting screen that shows up and has user clickable regions defined by anchors

    void LoadMock(AlgeApp* thiz, string titleTag, string tagSettings, string tagPointer, string tagIcon, string backgrnd) {
        app = thiz;
        
        thiz->refObject = startScreen.LoadIn(thiz,backgrnd);
        
        with thiz->AddResource(&titl, titleTag.c_str(), 1.2);
		titleObject = &_; 
        _.JuiceType = JuiceTypes::JUICE_PULSATE;
        _.AddInstance(f2(_.pos.x - 3, _.pos.y - 3))->color = f3(0.7, 0.7, 0.7); //grey shadow
        _.AddInstance(f2(_.pos.xy()));
        _with
      
        score.pos.y = 0.05 * thiz->bottomSide;
        score.pos.x = 0.85 * thiz->rightSide;
        thiz->AddObject(&score);
        
        with dPad.LoadIn(thiz);
		dPad.JuiceType = 0;// JuiceTypes::JUICE_SCALE_IN;
        _.color = f3( 1,1,1);
        _with
        
        settings.LoadIn(thiz,  tagSettings,  tagPointer,  tagIcon, &startScreen.bg, false);
    }
    
    void processInput(PEG::CMD* cmd, float deltaT) {
       
        if (!app) return;
		if (cmd->command == CMD_SCREENSIZE) {
			RepositionObjects(app->rightSide, app->bottomSide);
			settings.RepositionObject(app->rightSide, app->bottomSide);
		}
        
        if (cmd->command == CMD_SETTINGS_SCREEN) {
            if (cmd->i1 == 1) {
                ShowTitle(false);
                startScreen.SetVisible(false);
                //startScreen.bg.Show();              
            }
            if (cmd->i1 == 2) {
                ShowTitle(true);
                startScreen.SetVisible(true);
                //startScreen.bg.Show();
            }
        }
        
        if (cmd->command == CMD_TOUCH_START)
            if (app->onTouched("settings_icon")) {
                settings.SetVisible(true);
            }
        
        if (cmd->command == CMD_GAMEPAD_EVENT && cmd->i1 == MyGamePad::EventTypes::PAD) {
            if (cmd->i2 == MyGamePad::EventCodes::PAD_LT || cmd->i2 == MyGamePad::EventCodes::PAD_RT) {
                startScreen.bg.m_touchedX = (cmd->i2 == MyGamePad::EventCodes::PAD_LT? app->leftSide:app->rightSide);
            }
            if (cmd->i2 == MyGamePad::EventCodes::PAD_UP || cmd->i2 == MyGamePad::EventCodes::PAD_DN) {
                startScreen.bg.m_touchedY = (cmd->i2 == MyGamePad::EventCodes::PAD_UP ? app->topSide : app->bottomSide);
            }
        }
        
        if (settings.m_visible) settings.processInput(cmd->command, i2(cmd->i1, cmd->i2));
       
    }
    
    void ShowTitle(bool visible = true) {
        if (visible) {
            titl.Show();
            titl.getInstancePtr(1)->hidden = false;
        }
        else {
            titl.Hide();
            titl.getInstancePtr(1)->hidden = true;
        }
    }
    
    void RepositionObjects(int rightSide, int bottomSide) {
		const int shadowSize = 3;
		for (int i = 0; i < 2; i++) {
			titl.getInstancePtr(i)->pos.x = rightSide / 2.0 + i * shadowSize;
			titl.getInstancePtr(i)->pos.y = bottomSide / 2.0 + i * shadowSize;
		}
        startScreen.ratings.pos.y = 0.1 * bottomSide;
        startScreen.start.pos.x = 0.5 * rightSide;
		startScreen.ratings.pos.x = startScreen.start.pos.x;
        startScreen.start.pos.y = 0.9 * bottomSide;

        score.pos.y = 0.05 * bottomSide;
        score.pos.x = 0.85 * rightSide;

        dPad.pos.x = rightSide - 64;
        dPad.pos.y = bottomSide / 2;
    }
};

