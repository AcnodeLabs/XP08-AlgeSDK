#ifndef XML_READER_INCLUDED 
#define XML_READER_INCLUDED 
//XMLReader.hpp Code
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#undef pi
#include "rapidxml/rapidxml.hpp"
using namespace std;
using namespace rapidxml;


class XmlReader {
    xml_document<> doc;
    xml_node<>* root_node = NULL;

    public:
    XmlReader() {
        
    }

    ~XmlReader() {
        
    }

    //Place your xaml file in Data then 
    // xxx.Init(string(path) + "/MainWindow.xaml");

    string m_path;
    string m_xamlFile;

    XmlReader* Init(string path) {
        m_path = path;
        activeStackingOrientationHorizontal = false;
        return this;
    }

    int ival(map<string, string> &attributes, string keyname) {
        int ret = -1;
        try {
            if (attributes.find(keyname) != attributes.end()) {
                ret = atoi(string(attributes.find(keyname)->second).c_str());
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
    short ic,is, iinp, ib, il, nc, nr;
    string Orientation;
    bool activeStackingOrientationHorizontal;
    bool checkboxes[32];
    int isliders[12];
    bool isbtnpressed[32];
    string labels[12];

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
        return col;
    }

    void pushForeground(string node,string fg) {
        ImGui::PushStyleColor(ImGuiCol_Text, colvec0(fg));
    }
    void pushBackground(string node, string bg) {
        if (node=="Window")
            ImGui::PushStyleColor(ImGuiCol_WindowBg, colvec0(bg));
        else
            ImGui::PushStyleColor(ImGuiCol_Button, colvec0(bg));
    }

    void popForeground(string fg) {
        ImGui::PopStyleColor();
    }
    void popBackground(string bg) {
        ImGui::PopStyleColor();
    }
    
    int WindowWidth;
    int WindowHeight;
    string WindowFont;
    float WindowFontSize;


    void onNodeAttributes(string parent, string node, map<string, string> attributes) {

        string Title = sval(attributes, "Title");
        string Content = sval(attributes, "Content");

        string Foreground = col0(sval(attributes, "Foreground"));
        string Background = col0(sval(attributes, "Background"));

        string FontSize = sval(attributes, "FontSize");


        pushBackground(node, Background);//then
        pushForeground(node, Foreground);

        //     ImGui::PushStyleColor(ImGuiCol_Button, colvec0(Background));

       //      ImGui::PushStyleColor(ImGuiCol_Button, colvec0(Foreground));
       //      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colvec0(Background));
       //      ImGui::PushStyleColor(ImGuiCol_ButtonActive, colvec0(Background));

        Orientation = sval(attributes, "Orientation");//StackPanel

        if (Orientation == "") Orientation = "Vertical";

        string Text = sval(attributes, "Text");//for TextBox

        int Width = ival(attributes, "Width");
        int Height = ival(attributes, "Height");
        int Maximum = ival(attributes, "Maximum");//for slider
        int Minimum = ival(attributes, "Minimum");//for slider

        if (Maximum == -1) Maximum = 10; //slider defaults as in xaml
        if (Minimum == -1) Minimum = 0;


#define INP_TEXT_LEN 256
        static char inputtext[12][INP_TEXT_LEN];
        static bool default_of_input_text_is_set[12] = { false,false, false, false, false, false, false, false, false, false, false, false };

        if (parent == "" && node == "Window") {
            ic = 0; is = 0; iinp = 0; ib = 0; il = 0; nr = 0; nc = 0;
            ImGui::Begin(Title.c_str());
            if (Width > -1 && Height >= -1) {
                WindowFont = sval(attributes, "FontFamily");
                WindowFontSize = float(ival(attributes, "FontSize"));
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

        if (node == "Button") {
            isbtnpressed[ib] = ImGui::Button(Content.c_str());
            ib++;
        }

        if (node == "Label") {
            ImGui::Text("%s", labels[il].length() ? labels[il].c_str() : Content.c_str());
            il++;
        }

        if (node == "CheckBox") ImGui::Checkbox(Content.c_str(), &checkboxes[ic++]);
        if (node == "Slider") {
            ImGui::SliderInt(Content.c_str(), &isliders[is] ,Minimum, Maximum, "%d");
            is++;
        }
        


        
        if (node == "_TextBox") {//disrupts slider
            if (!default_of_input_text_is_set[iinp]) { strcpy(inputtext[iinp], Text.c_str()); default_of_input_text_is_set[iinp]=true; }
            ImGui::InputText("", inputtext[iinp], INP_TEXT_LEN);
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

        if (node == "StackPanel") {
            if (Orientation == "Horizontal") activeStackingOrientationHorizontal = true;
        }

        popForeground(Foreground);
        popBackground(Background);

  //    ImGui::PopStyleColor(3);
 //     ImGui::PopStyleColor(ImGuiCol_Button);
    }

    int InputTextCbk(ImGuiInputTextCallbackData* data) {
        return -1;
    }


    void IterateComplete(string parent, string node) {
        if (node == "Window" || node =="ContentPage") { ImGui::End(); }
        if (node == "StackPanel") { 
            Orientation = "Vertical";
            activeStackingOrientationHorizontal = false;
        }
    }

    void IterateOver(xml_node<>* node, xml_node<>* parent = nullptr) {
        // Iterate over the child nodes
        
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
                IterateOver(child_node, node);
            }
        }
        IterateComplete(string(parent?parent->name():""), string(node->name()));
    }
    vector<char> duffer;

    void Load(string xamlfile) {
            m_xamlFile = xamlfile;
            // Read the sample.xml file
            ifstream theFile(m_path + "/" + m_xamlFile);
            vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
            buffer.push_back('\0');
            for (unsigned int i = 0; i < buffer.size(); i++)
                duffer.push_back(buffer[i]);
            doc.parse<0>(&duffer[0]);
            root_node = doc.first_node("Window");
            if (!root_node) root_node = doc.first_node("ContentPage");//Maui
    }

    void Render() {
        ic = 0;
        if (root_node!=NULL) 
            IterateOver(root_node);
    }

};
// End of UIMLReader.hpp code
#endif