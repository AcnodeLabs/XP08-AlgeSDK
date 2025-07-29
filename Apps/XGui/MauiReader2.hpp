#ifndef MAUI_READER_INCLUDED 
#define MAUI_READER_INCLUDED 
//MauiReader.hpp Code
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#undef pi
#include "rapidxml/rapidxml.hpp"

using namespace std;
using namespace rapidxml;

class XGui {
    static vector<string> xr;    
public:

    class CenterAndExpand {
        signed short centerLine;
        signed short m_spacing;
        signed short cursorX;
    public:
        
        vector<short> cursorPos;
        void addFirstElem(short centerX, short Width, short spacing = 0) {
            centerLine = centerX;
            m_spacing = spacing;
            cursorX = centerLine - Width / 2;
            cursorPos.push_back(cursorX);
            
        }

        void addElem(short Width) {
            cursorPos.push_back(cursorX);
            for (short i = 0; i < (short)cursorPos.size(); i++) {
                cursorPos[i] -= (Width + m_spacing);
            }
            
        }

    };

    class Elem {
    public:
        class semanticProperties {
        public:
            string HeadingLevel, Description, Hint;
        };
        Elem(string xName) { 
            this->xName = xName;
        }
        Elem() {}
        string xName, Text, Source, TextTransform, Color, BackgroundColor, MapType;
        string VerticalOptions, HorizontalOptions;
        string HeightRequest, WidthRequest;
        string Focused, Clicked;
        string FontSize,FontAttributes;
        semanticProperties SemanticProperties;

        void clear() {
            memset(this, sizeof(Elem), 0);
            tabs = 4;
        }

        string asStringLines() {
            string ret = "";
            vector<string> rec = asStringVec();
            ret += tab();
            for (auto x : rec) {
                ret += x + " ";
            }
            return ret;
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

        void fillfromXGTformat(string props) {
            if (props[0] == '|') {
                xName = props; Text = props; return;
            }
            vector<string> pp = split(props, ',');
            for (auto p : pp) {
                vector<string> pepa, pepb;
                pepa = split(p, ':');
                pepb = split(p, '=');
                vector<string>& ppa = pepa.size() == 2 ? pepa : pepb;
                auto ppa1 = trim(ppa[1]);
                ppa[1] = ppa1;
                if (ppa.size() != 2) break;
                if (ppa[0]._Starts_with("so"))
                    this->Source = ppa[1];
                if (ppa[0]._Starts_with("wr")) this->WidthRequest = ppa[1];
                if (ppa[0]._Starts_with("hr")) this->HeightRequest = ppa[1];
                if (ppa[0]._Starts_with("co")) this->Color = ppa[1];
                if (ppa[0]._Starts_with("bc")) this->BackgroundColor = ppa[1];
                if (ppa[0]._Starts_with("fs")) this->FontSize = ppa[1];
                if (ppa[0]._Starts_with("cl")) this->Clicked = ppa[1];
                if (ppa[0]._Starts_with("ho")) this->HorizontalOptions = ppa[1];
                if (ppa[0]._Starts_with("vo")) this->VerticalOptions = ppa[1];
                if (ppa[0]._Starts_with("te")) this->Text = ppa[1];
                if (ppa[0]._Starts_with("tx")) this->Text = ppa[1];
                if (ppa[0]._Starts_with("na")) {
                    this->xName = ppa[1]; if (xName._Starts_with("btn")) this->Clicked = "OnCounterClicked";//Can be improved
                }
            }
        }

        vector<string> asStringVec() {
            vector<string> me;
            me.clear();
            if (!xName.empty()) me.push_back("x:Name=\""+xName+"\"");
            if (!Text.empty()) me.push_back("Text=\"" + Text + "\"");
            if (!Source.empty()) me.push_back("Source=\"" + Source + "\"");
            if (!TextTransform.empty()) me.push_back("TextTransform=\"" + TextTransform + "\"");
            if (!BackgroundColor.empty()) me.push_back("BackgroundColor=\"" + BackgroundColor + "\"");
            if (!Color.empty()) me.push_back("Color=\"" + Color + "\"");

            if (!VerticalOptions.empty()) me.push_back("VerticalOptions=\"" + VerticalOptions + "\"");
            if (!HorizontalOptions.empty()) me.push_back("HorizontalOptions=\"" + HorizontalOptions + "\"");

            if (!HeightRequest.empty()) me.push_back("HeightRequest=\"" + HeightRequest + "\"");
            if (!WidthRequest.empty()) me.push_back("WidthRequest=\"" + WidthRequest + "\"");

            if (!Focused.empty()) me.push_back("Focused=\"" + Focused + "\"");
            if (!Clicked.empty()) me.push_back("Clicked=\"" + Clicked + "\"");

            if (!FontSize.empty()) me.push_back("FontSize=\"" + FontSize + "\""); else me.push_back("FontSize=\"16\"");
            if (!FontAttributes.empty()) me.push_back("FontAttributes=\"" + FontAttributes + "\"");

            if (!SemanticProperties.Description.empty()) me.push_back("SemanticProperties.Description=\"" + SemanticProperties.Description + "\"");
            if (!SemanticProperties.HeadingLevel.empty()) me.push_back("SemanticProperties.HeadingLevel=\"" + SemanticProperties.HeadingLevel + "\"");
            if (!SemanticProperties.Hint.empty()) me.push_back("SemanticProperties.Hint=\"" + SemanticProperties.Hint + "\"");

            return me;
        }

    };

    static ImU32 ColorFromName(string name) {
        float m = 255.;
        ImVec4 Color_CornFlowerBlue(130 / m, 163 / m, 255 / m, 1);
        ImVec4 Color_Black(0 / m, 0 / m, 0 / m, 1);
        ImVec4 Color_Red(255 / m, 0 / m, 0 / m, 1);
        ImVec4 Color_Green(0 / m, 255 / m, 0 / m, 1);
        ImVec4 Color_Blue(0 / m, 0 / m, 255 / m, 1);
        ImVec4 Color_White(255 / m, 255 / m, 255 / m, 1);
        ImVec4 Color_Grey(125 / m, 125 / m, 125 / m, 1);
        ImVec4* ColorRet = &Color_Grey;
        if (name == "CornFlowerBlue") { ColorRet = &Color_CornFlowerBlue; }
        if (name == "Black") { ColorRet = &Color_Black; }
        if (name == "Blue") { ColorRet = &Color_Blue; }
        if (name == "White") { ColorRet = &Color_White; }
        if (name == "Green") { ColorRet = &Color_Green; }
        if (name == "Red") { ColorRet = &Color_Red; }
        if (name == "CornFlowerBlue") { ColorRet = &Color_CornFlowerBlue; }
        return  ImGui::ColorConvertFloat4ToU32(*ColorRet);
    }

    static vector<string> head;
    static string tab() {string ret = ""; for (int i = 0; i < tabs; i++) ret += "\t";return ret;}
    static void push(string s) { xr.push_back(tab() + s); }
    static void pushh(string s) { head.push_back(tab() + s); }

    static short CalcTextSize(string Text, short iFontSize) {
        return ImGui::CalcTextSize(Text.c_str()).x / 18. * iFontSize;
    }

    static bool usesMaps() {
        string maptag = "maps:Map";
        bool ret = false;
        for (auto uu : xr) {
  //          if (reuse::xcontains(uu, maptag)) { ret = true; break; }
        }
        return true;
    }

    static void pushhead(string mclass_mpage, string mtitle) {
        head.clear();
        head.push_back("<?xml version=\"1.0\" encoding = \"utf-8\"?>");//no tabls allowed here
        pushh("<ContentPage xmlns=\"http://schemas.microsoft.com/dotnet/2021/maui\"");
        pushh("\t\t\txmlns:x=\"http://schemas.microsoft.com/winfx/2009/xaml\"");
        if (usesMaps()) pushh("\t\t\txmlns:maps=\"clr-namespace:Microsoft.Maui.Controls.Maps;assembly=Microsoft.Maui.Controls.Maps\"");
        pushh("\t\t\tx:Class=\"" + mclass_mpage + "\"");
        pushh("\t\t\tTitle=\"" + mtitle + "\">");
        pushh("<ScrollView>");
    }

    static void init(string mclass_mpage, string mtitle) {
        pushhead(mclass_mpage, mtitle);
        xr.clear();
        for (auto h : head) {
            push(h);
        }
        tabs = 1;
    }
    static int tabs;
    static void clear() { xr.clear(); }
    static void Begin(string mclass_mpage, string mtitle) {
        ImGui::Begin(mtitle.c_str());
        init(mclass_mpage, mtitle);
    }

    static void CenterFor(string Text) {
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x) * 0.5f - ImGui::CalcTextSize(Text.c_str()).x / 2);
    }

    static bool XGui::Button(XGui::Elem e) {
        if (e.HorizontalOptions == "Center" || e.HorizontalOptions == "" ) CenterFor(e.Text);
    XGui::push("<Button "+e.asStringLines() + "/>");
    if (e.Text == "") e.Text = e.xName;
    int textWidth = ImGui::CalcTextSize(e.Text.c_str()).x;
    int textHeight = ImGui::CalcTextSize(e.Text.c_str()).y;    
    bool ret = ImGui::Button(e.Text.c_str());
    Orientate(i2(textWidth, textHeight), e.HorizontalOptions, e.VerticalOptions);
    return ret;
    }

    static void XGui::Label(XGui::Elem e) {
        if (e.HorizontalOptions == "Center") CenterFor(e.Text);
        XGui::push("<Label " + e.asStringLines() + "/>");
        if (e.Text == "") e.Text = e.xName;
        int textWidth = ImGui::CalcTextSize(e.Text.c_str()).x;
        int textHeight = ImGui::CalcTextSize(e.Text.c_str()).y;
        ImGui::Text(e.Text.c_str());
        Orientate(i2(textWidth, textHeight), e.HorizontalOptions, e.VerticalOptions);
    }

    static void XGui::Map(XGui::Elem e) {
        if (e.HorizontalOptions == "Center") CenterFor(e.Text);
        XGui::push("<maps:Map " + e.asStringLines() + "/>");
        if (e.Text == "") e.Text = e.xName;
        int textWidth = ImGui::CalcTextSize(e.Text.c_str()).x;
        int textHeight = ImGui::CalcTextSize(e.Text.c_str()).y;
        ImGui::Text("--MAPCONTROL--");
        Orientate(i2(textWidth, textHeight), e.HorizontalOptions, e.VerticalOptions);
    }

    static void XGui::Entry(XGui::Elem e) {
        if (e.HorizontalOptions == "Center" || e.HorizontalOptions == "") CenterFor(e.Text);
        XGui::push("<Entry " + e.asStringLines() + "/>");
        static char buff[64];
        strcpy(buff, e.Text.c_str());
        int flags = 0;
        if (e.Text == "") e.Text = e.xName;
        int textWidth = ImGui::CalcTextSize(e.Text.c_str()).x;
        int textHeight = ImGui::CalcTextSize(e.Text.c_str()).y;
        
        ImGui::InputTextWithHint(e.Text.c_str(), e.SemanticProperties.Description.c_str(), buff, 16);
        Orientate(i2(textWidth, textHeight), e.HorizontalOptions, e.VerticalOptions);
    }

    static vector<string> orient;
    static string currentorient;
    static void XGui::PushOrientation(string hv) {
        orient.push_back(hv);
        currentorient = orient.at(orient.size()-1);
    }
    
    static void XGui::PopOrientation() {
        if (orient.size() > 0) {
            orient.pop_back();
            if (orient.size() == 0)
                currentorient = "";
            else 
                currentorient = orient.at(orient.size() - 1);
        }
    }
    static void XGui::Orientate(i2 elemSize, string horiz_alignment, string vert_alignment) {
        static int x=0, y=0;
        auto windowWidth = ImGui::GetWindowSize().x;
        auto windowHeight = ImGui::GetWindowSize().y;
        x = ImGui::GetCursorPosX();
        y = ImGui::GetCursorPosY();
        if (horiz_alignment == "Center") x = windowWidth * 0.5 - elemSize.x / 2;
        if (horiz_alignment == "End") x = windowWidth - elemSize.y ;
                
        if (currentorient == "vert") {
            y += elemSize.y;
        }
        if (currentorient == "horiz") {
            x += elemSize.x;
        }

        ImGui::SetCursorPosX(x);
        ImGui::SetCursorPosY(y);


    }
    static i2 XGui::Image(XGui::Elem e, CResourceManager* rm) {
        string imgname = split(e.Source, '.')[0];
        TextureImage* t = rm->getTexOf(imgname);
        int h = 100;
        int w = 100;
        int iHeightRequest = atoi(e.HeightRequest.c_str());
        if (iHeightRequest > 0) {
            h = iHeightRequest;
            w = int((t->width / t->height) * iHeightRequest);
        }
        else {
            h = t->height;
            w = t->width;
        }
        if (e.HorizontalOptions == "Center" || e.HorizontalOptions == "") {
            auto windowWidth = ImGui::GetWindowSize().x;
            ImGui::SetCursorPosX((windowWidth) * 0.5f - w / 2);
        }
       
        ImGui::Image((void*)(intptr_t)t->texID, ImVec2(w, h)); XGui::push("<Image " + e.asStringLines() + "/>");
        Orientate(i2(w, h), e.HorizontalOptions, e.VerticalOptions);
        return i2(w,h);
    }

    static void BoxView(Elem e) {
        static int top = 0, left = 0;
        XGui::push("<BoxView " + e.asStringLines() + "/>");
        int iHeightRequest = atoi(e.HeightRequest.c_str());
        int iWidthRequest = atoi(e.HeightRequest.c_str());
        ImGui::GetWindowDrawList()->AddRectFilled(
            ImVec2(left, top),
            ImVec2(left + iWidthRequest, top + iHeightRequest),
            ColorFromName(e.Color)
        );
        Orientate(i2(iWidthRequest, iHeightRequest), e.HorizontalOptions, e.VerticalOptions);

    }

    static void SetWindowSize(i2 sz) {
        ImGui::SetWindowSize(ImVec2(sz.x,sz.y));
  //      push("Width=" + to_string(sz.x) + "\tHeight=" + to_string(sz.y));
    }
    
    static bool once;

    static string getType(string xName) {
        if (xName._Starts_with("lbl") || xName._Starts_with("txt")) return "Label";
        if (xName._Starts_with("btn")) return "Button";
        if (xName._Starts_with("inp") || xName._Starts_with("ent")) return "Entry";
        if (xName._Starts_with("img")) return "Image";
        return "Label";
    }
    
    static string rtrim(string s) { return s.erase(s.find_last_not_of(" \n") + 1);}

    static string tagged(string xName, string bare) {
        string opts= "";
        if (xName._Starts_with("|")) {
            vector<string> attr = split(xName, '|');            
            if (attr.size() == 3) {
                opts = attr[2];
            }            
        }
        if (xName._Starts_with("|Horiz_Begin|")) return "<HorizontalStackLayout "+opts+">";
        if (xName._Starts_with("|Horiz_End|")) return "</HorizontalStackLayout " + opts + ">";
        if (xName._Starts_with("|Vert_Begin|")) return "<VerticalStackLayout " + opts + ">";
        if (xName._Starts_with("|Vert_End|")) return "</VerticalStackLayout " + opts + ">";
        string type = getType(xName);
        return "\t<" + type + " " + (bare) + "/>";
    }

    static void dumps(AlgeApp* app, vector<Elem> u, vector<string> c, string mpage, string mtitle, string where) {
        pushhead(mpage,mtitle);
        vector<string> eventhandlers;
        string sz = "";
        for (auto h : head) {
            sz += h + "\n";
        }
        for (auto e : u) {
            string r = e.asStringLines();
            sz += "\t"+tagged(e.xName, r) + "\n";
            if (e.Clicked!="") eventhandlers.push_back(e.Clicked);
        }
        sz += "\t</ScrollView>\n</ContentPage>";
        
        FILE* f = fopen(where.c_str(), "w");
        if (f) {
            fputs(sz.c_str(), f);
            fclose(f);
        }
        
        f = fopen((where+".xgt").c_str(), "w");
        if (f) {
            for (auto d : c) {
                if (d.length()>1) fputs((rtrim(d) + "\n").c_str(), f);
            }
            fclose(f);
        }
    }

    static void dumps(string where) {
        string dumps = "";
        for each (auto x in xr) {
            dumps += x + "\n";
        }
        if (!once) {
            FILE* f = fopen(where.c_str(), "w");
            if (f) {
                fputs(dumps.c_str(),f);
                fclose(f);
            }
            once = true;
        }
        return;
    }
    static void End() {
        ImGui::End();
        push("</ScrollView></ContentPage>");
    }
};
vector<string> XGui::xr;
vector<string> XGui::head;
vector<string> XGui::orient;
string XGui::currentorient;

int XGui::tabs = 0;
bool XGui::once = false;

class MauiReader {
    xml_document<> doc;
    xml_node<>* root_node = NULL;

    public:
    MauiReader() {
        
    }

    MauiReader(string BaseDir) {
        m_path = BaseDir;
    }

    ~MauiReader() {
        
    }

    //Place your xaml file in Data then 
    // xxx.Init(string(path) + "/MainWindow.xaml");

    string m_path;
    string m_xamlFile;
    CResourceManager* rm;
    i2 workingResolution;
    
    MauiReader* Init(string path, AlgeApp* that) {
        m_path = path;
        vector<string> pel = split(m_path, '/');
        usernamespace = pel[pel.size()-1];
        Init(that);
        return this;
    }
    
    AlgeApp* m_app;

    MauiReader* Init(AlgeApp* app) {
        m_app = app;
        this->rm = &(app->rm);
        app->coord.setOrientation(false);
        this->m_imHelper = &(app->imHelper);
        activeStackingOrientationHorizontal = false;
        workingResolution = i2(app->rightSide, app->bottomSide);
        return this;
    }

    int ival(map<string, string> &attributes, string keyname) {
        int ret = -1;
        try {
            if (attributes.find(keyname) != attributes.end()) {
                try {
                    ret = atoi(string(attributes.find(keyname)->second).c_str());
                }
                catch (...) {
                    ret = -1;
                }
            }
        }  
        catch (...) {}
        return ret;
    }

    string sval(map<string, string>& attributes, string keyname) {
        string ret = "";
        try {
            if (attributes.find(keyname) != attributes.end()) {
                ret = string(attributes.find(keyname)->second);
            }
        }
        catch (...) {}
        return ret;
    }
    short ic,is, iinp, ib, il, nc, nr, ii;
    string Orientation;
    bool activeStackingOrientationHorizontal;
    int VerticalStackLayout_Top[5];//Max Depth = 5
    int HorizontalStackLayout_Left[5];
    int VerticalStackLayout_Left[5];
    int HorizontalStackLayout_Top[5];
    short iVSL; //index of current vertical stacklayout to store/retireve Top and Left values
    short iHSL; //index of current horizontal stacklayout to store/retireve Top and Left values
    bool checkboxes[32];
    int isliders[12];
    bool isbtnpressed[32];//use isButtonPressed(frunction)
    string labels[12];
    string images[12];
    char inputtext[12][256];
    bool default_of_input_text_is_set[12] = { false,false, false, false, false, false, false, false, false, false, false, false };

    int hexa(char l, char m)
    {
        char num[3] = { l,m , 0 };
        int i, r, len, hex = 0;
        len = strlen(num);
        for (i = 0; num[i] != '\0'; i++)
        {
            len--;
            if (num[i] >= '0' && num[i] <= '9')
                r = num[i] - 48;
            else if (num[i] >= 'a' && num[i] <= 'f')
                r = num[i] - 87;
            else if (num[i] >= 'A' && num[i] <= 'F')
                r = num[i] - 55;
            hex += r * pow(16, len);
        }
        return hex;
    }

    ImVec4 colvec0(string H) {
        //H=HASHSIGN+RRGGBBAA
        bool invalid = H[0] != '#';
        if (invalid) return ImVec4(0.,0.,0.,0);
        auto i = ImVec4(hexa(H[3], H[4]) / 255.,hexa(H[5],H[6])/255., hexa(H[7], H[8])/255., hexa(H[1], H[2])/255.);//ARGB
        return i;
    }

    string col0(string col) {
        if (col[0] == '#') return col;
        //Common colors Specified in Words
        if (col == "White") return "#FFFFFFFF";
        if (col == "Black") return "#00000000";
        if (col == "CornFlowerBlue") {
            return "#6495EDFF";
        }
        return col;
    }

    
    int WindowWidth;
    int WindowHeight;
    string WindowFont;
    float WindowFontSize;
    ImVec4 bgvec;
    ImHelper* m_imHelper;
    int parentalSpacing = 0;
    int XinHSL;

    void CenterFor(string Text) {
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x) * 0.5f - ImGui::CalcTextSize(Text.c_str()).x / 2);
    }

    bool isButtonPressed(short index) {
        bool state = isbtnpressed[index];
        isbtnpressed[index] = false;
        return state;
    }

    void onNodeAttributes(string parent, string node, map<string, string> attributes) {

        string Title = sval(attributes, "x:Class");
        string Content = sval(attributes, "Content");
        string Source = sval(attributes, "Source");

        string HorizontalOptions = sval(attributes, "HorizontalOptions");
        string Foreground = col0(sval(attributes, "Foreground"));
        string Background = col0(sval(attributes, "Background"));
        string Color = sval(attributes, "Color");
        string ColorHex = col0(sval(attributes, "Color"));

        Orientation = sval(attributes, "Orientation");//StackPanel

        if (Orientation == "") Orientation = "Vertical";

        string Text = sval(attributes, "Text");//for TextBox

        int Width = ival(attributes, "Width");
        int Height = ival(attributes, "Height");
        int Maximum = ival(attributes, "Maximum");//for slider
        int Minimum = ival(attributes, "Minimum");//for slider
        int iFontSize = ival(attributes, "FontSize");
        int iSpacing = ival(attributes, "Spacing");
        if (iSpacing>0) parentalSpacing = iSpacing;
        if (iFontSize <= 0) iFontSize = 24;
        int iHeightRequest = ival(attributes, "HeightRequest");
        int iWidthRequest = ival(attributes, "WidthRequest");
        

        if (Maximum == -1) Maximum = 10; //slider defaults as in xaml
        if (Minimum == -1) Minimum = 0;


#define INP_TEXT_LEN 256
        

       
        if (parent == "" && node == "ContentPage") {
            ic = 0; is = 0; iinp = 0; ib = 0; il = 0; nr = 0; nc = 0; ii = 0;
            ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(255, 255, 255, 255));
            //ImGui::SetNextWindowPos(ImVec2(0,0));
        //    i2 sz = workingResolution.flip();//landscape
        //    ImGui::SetNextWindowSize(ImVec2(sz.x,sz.y));
           
            bgvec.x = 255;
            if (Width >= 0 && Height >= 0) {
                WindowFont = sval(attributes, "FontFamily");
                WindowFontSize = float(ival(attributes, "FontSize"));
                ImGui::SetNextWindowSize(ImVec2(Width, Height));
                WindowWidth = Width; WindowHeight = Height;
             
            }
            else {//If Not Defined
                i2 siz = m_app->getResolutionForGuiUsage();
                Width = siz.x;
                Height = siz.y;
                WindowFont = "Roboto-Bold";
                WindowFontSize = 32;
                ImGui::SetNextWindowSize(ImVec2(Width, Height));
                WindowWidth = Width; WindowHeight = Height;
            }
            XGui::Begin(Title, Title);
        }

        if (node == "Label" && Width == -1) {
            Width = XGui::CalcTextSize(Text, iFontSize);
        }
        if (Width == -1 && (node == "Image" || node == "maps:Map") ) {
            short w=0;
            if (w==0 && iHeightRequest > 0) w = iHeightRequest;
            if (w==0 && iWidthRequest > 0) w = iWidthRequest;
            Width = w;
        }

        if (parent == "Grid.RowDefinitions") {
            if (node == "RowDefinition") {
                nr++;
            }
        }

        if (parent == "Grid.ColumnDefinitions") {
            if (node == "ColumnDefinition") {
                nc++;
            }
        }

        if (parent == "Grid") {
            static int Grid_Row = -1;
            int Grid_Row_Now = ival(attributes, "Grid.Row");
            activeStackingOrientationHorizontal = (Grid_Row_Now == Grid_Row);
            Grid_Row = Grid_Row_Now;
        
            if (activeStackingOrientationHorizontal) {
                if (nc > 0) ImGui::SameLine();// 0, WindowWidth / nc / 2);//TODO
            }
        }

        static XGui::CenterAndExpand cae;
        static string ParentHorizontalOptions;
        static short hslElemCounter = 0;

        if (parent != node && node == "HorizontalStackLayout") {
            ParentHorizontalOptions = HorizontalOptions;
            hslElemCounter = 0;
        }
               
        if (parent == "HorizontalStackLayout" && ParentHorizontalOptions == "CenterAndExpand") {
            hslElemCounter++;
            if (hslElemCounter == 1) cae.addFirstElem(ImGui::GetWindowSize().x / 2, Width);
            else cae.addElem(Width);
            ImGui::SetCursorPosX(cae.cursorPos[hslElemCounter-1]);
        }


        if (HorizontalOptions=="Center") {
            auto windowWidth = ImGui::GetWindowSize().x;
            auto textWidth = ImGui::CalcTextSize(Text.c_str()).x / 18. * iFontSize;
            ImGui::SetCursorPosX((windowWidth) * 0.5f - textWidth / 2);
        }

        //// PREREQSUISITE OF DRAWABLES

        i2 siz = m_app->coord.getResolutionForGuiUsage();
        if (iWidthRequest <= 0) iWidthRequest = siz.x;
        if (iHeightRequest <= 0) iHeightRequest = siz.y;

        int top;
        int left;

        {
            if (parent == "VerticalStackLayout") top = VerticalStackLayout_Top[iVSL];
            if (parent == "HorizontalStackLayout") top = HorizontalStackLayout_Top[iHSL];

            if (parent == "VerticalStackLayout") left = VerticalStackLayout_Left[iVSL];
            if (parent == "HorizontalStackLayout") left = HorizontalStackLayout_Left[iHSL];

            if (node == "VerticalStackLayout")  iVSL++;
            if (node == "HorizontalStackLayout") iHSL++;

        }
        if (HorizontalOptions == "") HorizontalOptions = "Center";

        if (parent == "HorizontalStackLayout") ImGui::SameLine();
        
        /// DRAWABLES
        if (iFontSize > 0) {
            ImGui::PushFont(m_imHelper->getImFont(iFontSize));
        }

        if (node == "Button") {
            //void XGui::Button(string text, string HorizontalOptions, string VerticalOptions) {
            if (HorizontalOptions == "") HorizontalOptions = "Center";
            if (HorizontalOptions._Starts_with("Center") ) CenterFor(Text);
            XGui::Elem e; 
            e.Text = Text;
            e.HorizontalOptions = HorizontalOptions;
            isbtnpressed[ib] = XGui::Button(e);
            ib++;
        }

        if (node == "Label") {
            if (labels[il].length()>0) Text = labels[il];//if label has been filled yet pick text from defualt xaml def
            XGui::Elem e; e.Text = string(labels[il].length() ? labels[il].c_str() : Text.c_str());
            XGui::Label(e); 
            il++;
        }

        if (node == "maps:Map") {
            XGui::Elem e; e.HeightRequest = to_string(iHeightRequest); e.HorizontalOptions = HorizontalOptions;
            XGui::Map(e);
            ii++;
        }

        if (node == "Image") {
            XGui::Elem e; e.Source = Source; e.HeightRequest = to_string(iHeightRequest); e.HorizontalOptions = HorizontalOptions;
            i2 size = XGui::Image(e, rm);
            ii++;
            XinHSL += size.x / 2;
        }

        if (node == "CheckBox") ImGui::Checkbox(Content.c_str(), &checkboxes[ic++]);
        if (node == "Slider") {
            char sliderName[16] = {'s',0};//to pass some thing else crash
            ImGui::SliderInt(Content.size()?Content.c_str():sliderName, &isliders[is] ,Minimum, Maximum, "%d"); 
            XGui::push("<Slider Max=\"" + to_string(Maximum) +" Min="+to_string(Minimum) + "\"/>");
            is++;
        }
                
        if (node == "BoxView") {
            XGui::Elem e;
            e.WidthRequest = to_string(iWidthRequest);
            e.HeightRequest = to_string(iHeightRequest);
            e.Color = Color;
            XGui::BoxView(e);
        }

        if (node == "Entry") {//disrupts slider
            if (HorizontalOptions == "Center") CenterFor(Text);
            
            if (!default_of_input_text_is_set[iinp]) { strcpy(inputtext[iinp], Text.c_str()); default_of_input_text_is_set[iinp]=true; }
         //   PushItemWidth(-1);
            
            XGui::Elem e; e.Text = "Text";
            XGui::push("<Entry "+e.asStringLines() + "/>");
            ImGui::InputText("##label", inputtext[iinp], INP_TEXT_LEN, ImGuiInputTextFlags_AutoSelectAll
                );
            
          //  PopItemWidth();
            iinp++;
        }

        if (node == "ListBox") {
            {
                // Using the _simplified_ one-liner ListBox() api here
                // See "List boxes" section for examples of how to use the more flexible BeginListBox()/EndListBox() api.
                const char* items[] = { "Apple", "Banana", "Cherry", "Kiwi", "Mango", "Orange", "Pineapple", "Strawberry", "Watermelon" };
                static int item_current = 1;
                ImGui::ListBox("", &item_current, items, IM_ARRAYSIZE(items), 4);
                XGui::push("<ListBox Items=\"" + string(items[0]) + "\"/>");
            }
        }
        //POST REQUISITES OF DRWABLES
        {
            if (parent == "VerticalStackLayout") {
                VerticalStackLayout_Top[iVSL] += iHeightRequest;
                
            }
            if (parent == "HorizontalStackLayout") {
                HorizontalStackLayout_Left[iHSL] += iWidthRequest;               
            }
        }

        if (iFontSize > 0) {
            ImGui::PopFont();
        }
        /// END OF DRAWABLES 

        if (node == "StackPanel") {
            if (Orientation == "Horizontal") activeStackingOrientationHorizontal = true;
        }

        if (parent == "VerticalStackLayout") {
            ImGui::Dummy(ImVec2(0.0f, parentalSpacing/1.0));
        }


  //    ImGui::PopStyleColor(3);
 //     ImGui::PopStyleColor(ImGuiCol_Button);
    }

    int InputTextCbk(ImGuiInputTextCallbackData* data) {
        return -1;
    }

  

    void IterateComplete(string parent, string node) {
        if (node =="ContentPage") { 
            ImGui::PopStyleColor();
            XGui::End(); 
        }
        if (node == "VerticalStackLayout") { 
            Orientation = "Vertical";
            activeStackingOrientationHorizontal = false;
            iVSL--;
        }
        if (node == "HorizontalStackLayout") {
            Orientation = "Vertical";
            activeStackingOrientationHorizontal = false;
            iHSL--;
        }
    }

    void resetControlValues() {
        iVSL = 0;
        iHSL = 0;
        for_i(5)
            VerticalStackLayout_Top[i] = 0;
            VerticalStackLayout_Left[i] = 0;
            HorizontalStackLayout_Top[i] = 0;
            HorizontalStackLayout_Left[i] = 0;
        _for
    }

    void IterateOver(xml_node<>* node, xml_node<>* parent = nullptr, bool firsttime=true) {
        // Iterate over the child nodes
        if (firsttime) {
            resetControlValues();
        }
        std::map<string, string> attributes;

        for (xml_attribute<>* attr = node->first_attribute();
            attr; attr = attr->next_attribute())
        {
            //onNodeAttribute(parent?string(parent->name()):"", string(node->name()), string(attr->name()), string(attr->value()));            
            attributes.insert(std::make_pair(string(attr->name()), string(attr->value())));
        }

        onNodeAttributes(parent ? string(parent->name()) : "", string(node->name()), attributes);

        if (node) {
            // Iterate over the child nodes
            for (xml_node<>* child_node = node->first_node(); child_node; child_node = child_node->next_sibling())
            {
                IterateOver(child_node, node,false);
            }
        }
        IterateComplete(string(parent?parent->name():""), string(node->name()));
    }
    vector<char> cache;
    bool modeHotReload = false;
    string hotreloadxaml = "";

    vector<XGui::Elem> getCacheAsElem() {
   
    }

    string getCacheAsString() {
        string lines= "";
        int i = 0;
        for (auto c : cache) {
            lines += c;
        }
        return lines;
    }

    string usernamespace;

    void LoadXGT(string xamlfile, bool hotreload = false) {

        m_xamlFile = xamlfile;
        if (hotreload) {
            modeHotReload = true;
            hotreloadxaml = xamlfile;
            return;
        }
        ReloadXGT(xamlfile);
    }

    void Load(string xamlfile, bool hotreload = false) {

        m_xamlFile = xamlfile;
            if (hotreload) {
                modeHotReload = true;
                hotreloadxaml = xamlfile;
                return;
            }
            Reload(xamlfile);
    }

    void ReloadXGT(string m_xamlFile) {
    
    }

   void Reload(string m_xamlFile) {
   
       ifstream theFile(m_path + "/" + m_xamlFile);
        vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
        buffer.push_back('\0');
        for (unsigned int i = 0; i < buffer.size(); i++)
            cache.push_back(buffer[i]);
        doc.parse<0>(&cache[0]);
        try {
            root_node = doc.first_node("ContentPage");//Maui
        }
        catch (...) {
        }
    }

    void Render(int interval_sec= 15) {
        ic = 0;
        static int hcnt = 0;
        int interval_tick = interval_sec * 60;//60fps assumed
        if (root_node!=NULL) 
            IterateOver(root_node);
        if (modeHotReload) {
            cache.clear();
            doc.clear();
            hcnt++;
            if (hcnt > interval_tick) {
                Reload(hotreloadxaml);
                hcnt = 0;
            }

        }
    }

};
// End of UIMLReader.hpp code
#endif