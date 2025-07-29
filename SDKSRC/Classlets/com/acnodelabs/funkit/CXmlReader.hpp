//XMLReader.hpp Code

#ifdef NO_IMGUI

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#undef pi
#include "rapidxml/rapidxml.hpp"
using namespace std;
using namespace rapidxml;

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Simple helper function to load an image into a OpenGL texture with common settings
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}


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

    static string CSProjDirectory;

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
    short ic,is, iinp, ib, il, nc, nr, im;
    string Orientation;
    bool activeStackingOrientationHorizontal;
    bool checkboxes[32];
    int isliders[12];
    bool isbtnpressed[32];
    string labels[12];
    map<string, int> identifiers[32];//type:name,identifier

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
        if (col == "Black") return "#000000FF";
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

    map<string, string> foregrounds;
    map<string, string> backgrounds;

    bool image_texture_loaded[8] = { false,false,false,false,false, false, false, false };
    GLuint image_texture[8] = {0,0,0,0,0,0};

    void DrawImage(string src, int w, int h) {
        string fullpath = m_path +"/"+ CSProjDirectory +"/" + src;
        if (!image_texture_loaded[im]) {
            image_texture_loaded[im] = LoadTextureFromFile(fullpath.c_str(), &image_texture[im], &w, &h);
      //      if (image_texture_loaded[im]==false) ImGui::Text(("Error Loading: " + fullpath).c_str());
            
        }
        if (image_texture_loaded[im] == false) 
           ImGui::Text(("Error Loading: " + fullpath).c_str()); 
        else
            ImGui::Image((void*)(intptr_t)image_texture[im], ImVec2(w, h));
        if (image_texture_loaded[im]) im++; //Multiple images Not yet Working
    }

    void onNodeAttributes(xml_node<>* parent_node, xml_node<>* this_node, map<string, string> attributes) {
        
        string parent = parent_node ? string(parent_node->name()) : "";
        string node = string(this_node->name());

        string Title = sval(attributes, "Title");
        string Content = sval(attributes, "Content");

        string Foreground = col0(sval(attributes, "Foreground"));
        string Background = col0(sval(attributes, "Background"));

        string Name = sval(attributes, "x:Name");

 /*
        if (Foreground.length() > 0)
            foregrounds.insert({ node, Foreground });
        else try {
            auto prnt = foregrounds.find(parent);
            if (prnt!=foregrounds.end()) Foreground = prnt->second;
        }
        catch (...) {}

        if (Background.length() > 0)
            backgrounds.insert({ node, Background });
        else try {
            auto prnt = backgrounds.find(parent);
            if (prnt != backgrounds.end()) Background = prnt->second;
        }
        catch (...) {}
*/
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
            ic = 0; is = 0; iinp = 0; ib = 0; il = 0; nr = 0; nc = 0; im = 0;
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


        if (node == "Image") {
            string src = sval(attributes, "Source");
            int w = Width;
            int h = Height;
            DrawImage(src, w, h);
            identifiers->insert({ "Image:" + Name,0 });
        }

        if (node == "Button") {
            isbtnpressed[ib] = ImGui::Button(Content.c_str());
            identifiers->insert({ "Button:" + Name,ib });
            ib++;
        }

        if (node == "Label") {
            ImGui::Text(labels[il].length() ? labels[il].c_str() : Content.c_str());
            identifiers->insert({"Label:"+Name,il});
            il++;
        }

        if (node == "CheckBox") ImGui::Checkbox(Content.c_str(), &checkboxes[ic++]);
        if (node == "Slider") {
            identifiers->insert({ "Slider:" + Name,is });
            ImGui::SliderInt(Content.c_str(), &isliders[is] ,Minimum, Maximum, "%d");
            is++;
        }
        


        
        if (node == "_TextBox") {//disrupts slider
            if (!default_of_input_text_is_set[iinp]) { strcpy(inputtext[iinp], Text.c_str()); default_of_input_text_is_set[iinp]=true; }
            ImGui::InputText("", inputtext[iinp], INP_TEXT_LEN);
            identifiers->insert({ "TextBox:" + Name,iinp });
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

    int getControlIdByName(string type, string name) {
        auto ids = identifiers->find(type + ":" + name);
        return (ids->second);
    }

    void IterateComplete(string parent, string node) {
        if (node == "Window") {ImGui::End();}
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

       
        onNodeAttributes(parent, node, attributes);

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
            m_xamlFile = CSProjDirectory + "/" + xamlfile;
            // Read the sample.xml file
            ifstream theFile(m_path + "/" + m_xamlFile);
            vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
            buffer.push_back('\0');
            for (unsigned int i = 0; i < buffer.size(); i++)
                duffer.push_back(buffer[i]);
            doc.parse<0>(&duffer[0]);
            root_node = doc.first_node("Window");
    }

    void Render() {
        ic = 0;
        if (root_node!=NULL) 
            IterateOver(root_node);
    }

};

string XmlReader::CSProjDirectory = "AppViews/AppViews";

// End of UIMLReader.hpp code
#endif
