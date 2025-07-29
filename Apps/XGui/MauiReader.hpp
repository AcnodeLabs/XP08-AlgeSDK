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

class XamlGen {
    
};


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
    bool isbtnpressed[32];
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

    void CenterFor(string Text) {
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x) * 0.5f - ImGui::CalcTextSize(Text.c_str()).x / 2);
    }

    ImU32 ColorFromName(string name) {
        float m = 255.;
        ImVec4 Color_CornFlowerBlue(130 / m, 163 / m, 255 / m, 1);
        ImVec4 Color_Black(0 / m, 0 / m, 0 / m, 1);
        ImVec4 Color_Red(255 / m, 0 / m, 0 / m, 1);
        ImVec4 Color_Green(0 / m, 255 / m, 0 / m, 1);
        ImVec4 Color_Blue(0 / m, 0 / m, 255 / m, 1);
        ImVec4 Color_White(255 / m, 255 / m, 255 / m, 1);
        ImVec4 Color_Grey(125 / m, 125 / m, 125 / m, 1);
        ImVec4* ColorRet = &Color_Grey;
        if (name == "CornFlowerBlue") { ColorRet = &Color_CornFlowerBlue;}
        if (name == "Black") { ColorRet = &Color_Black; }
        if (name == "Blue") { ColorRet = &Color_Blue; }
        if (name == "White") { ColorRet = &Color_White; }
        if (name == "Green") { ColorRet = &Color_Green; }
        if (name == "Red") { ColorRet = &Color_Red; }
        if (name == "CornFlowerBlue") { ColorRet = &Color_CornFlowerBlue; }
        return  ImGui::ColorConvertFloat4ToU32(*ColorRet);
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
        if (iFontSize <= 0) iFontSize = 12;
        int iHeightRequest = ival(attributes, "HeightRequest");
        int iWidthRequest = ival(attributes, "WidthRequest");
        

        if (Maximum == -1) Maximum = 10; //slider defaults as in xaml
        if (Minimum == -1) Minimum = 0;


#define INP_TEXT_LEN 256
        

        if (parent == "" && node == "ContentPage") {
            ic = 0; is = 0; iinp = 0; ib = 0; il = 0; nr = 0; nc = 0; ii = 0;
            ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(255, 255, 255, 255));
            ImGui::SetNextWindowPos(ImVec2(0,0));
            i2 sz = workingResolution.flip();//landscape
            ImGui::SetNextWindowSize(ImVec2(sz.x,sz.y));
            ImGui::Begin(Title.c_str());
            bgvec.x = 255;
            if (Width >= -1 && Height >= -1) {
                WindowFont = sval(attributes, "FontFamily");
                WindowFontSize = float(ival(attributes, "FontSize"));
                ImGui::SetWindowSize(ImVec2(Width, Height));
                WindowWidth = Width; WindowHeight = Height;
             
            }
            else {//If Not Defined
                i2 siz = m_app->coord.getResolutionForGuiUsage();
                Width = siz.x;
                Height = siz.y;
                WindowFont = "Roboto-Bold";
                WindowFontSize = 32;
                ImGui::SetWindowSize(ImVec2(Width, Height));
                WindowWidth = Width; WindowHeight = Height;
            }
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

        if (HorizontalOptions == "Center") {
            auto windowWidth = ImGui::GetWindowSize().x;
            ImGui::SetCursorPosX((windowWidth) * 0.5f - ImGui::CalcTextSize(Text.c_str()).x / 2);
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

        /// DRAWABLES
        if (iFontSize > 0) {
            ImGui::PushFont(m_imHelper->getImFont(iFontSize));
        }

        if (node == "Button") {
            if (HorizontalOptions == "Center") CenterFor(Text);
            isbtnpressed[ib] = ImGui::Button(Text.c_str());
            ib++;
        }

        if (node == "Label") {
            if (labels[il].length()>0) Text = labels[il];//if label has been filled yet pick text from defualt xaml def
            if (HorizontalOptions == "Center") CenterFor(Text);
            ImGui::Text("%s", labels[il].length() ? labels[il].c_str() : Text.c_str());
            il++;
        }

        if (node == "Image") {
            string imgname = split(Source, '.')[0];
            TextureImage* t = rm->getTexOf(imgname);
            int h = 100;
            int w = 100;
            if (iHeightRequest > 0) {
                h = iHeightRequest;
                w = int((t->width / t->height) * iHeightRequest);
            }
            else {
                h = t->height;
                w = t->width;
            }
            if (HorizontalOptions == "Center") {
                auto windowWidth = ImGui::GetWindowSize().x;
                ImGui::SetCursorPosX((windowWidth) * 0.5f - w/2);
            }
            ImGui::Image((void*)(intptr_t)t->texID, ImVec2(w, h));
            ii++;
        }

        if (node == "CheckBox") ImGui::Checkbox(Content.c_str(), &checkboxes[ic++]);
        if (node == "Slider") {
            ImGui::SliderInt(Content.c_str(), &isliders[is] ,Minimum, Maximum, "%d");
            is++;
        }
                
        if (node == "BoxView") {

            ImGui::GetWindowDrawList()->AddRectFilled(
                ImVec2(left, top), 
                ImVec2(left + iWidthRequest, top + iHeightRequest), 
                ColorFromName(Color)
            );

        }

        if (node == "Entry") {//disrupts slider
            if (HorizontalOptions == "Center") CenterFor(Text);
            
            if (!default_of_input_text_is_set[iinp]) { strcpy(inputtext[iinp], Text.c_str()); default_of_input_text_is_set[iinp]=true; }
         //   PushItemWidth(-1);
            
            
            
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
            ImGui::End(); 
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
    vector<char> duffer;
    bool modeHotReload = false;
    string hotreloadxaml = "";

    void Load(string xamlfile, bool hotreload = false) {
       
            m_xamlFile = xamlfile;
            if (hotreload) {
                modeHotReload = true;
                hotreloadxaml = xamlfile;
                return;
            }
            Reload(xamlfile);
    }

   void Reload(string m_xamlFile) {
   
       ifstream theFile(m_path + "/" + m_xamlFile);
        vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
        buffer.push_back('\0');
        for (unsigned int i = 0; i < buffer.size(); i++)
            duffer.push_back(buffer[i]);
        doc.parse<0>(&duffer[0]);
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
            duffer.clear();
            doc.clear();
            hcnt++;
            if (hcnt==1 || hcnt > interval_tick) {
                Reload(hotreloadxaml);
                hcnt = 0;
            }

        }
    }

};
// End of UIMLReader.hpp code
#endif
