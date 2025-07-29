
// XGui stuff
#include "../../../imgui/imgui.h"
#include "../../../imgui/backends/imgui_impl_glut.h"
#include "../../../imgui/backends/imgui_impl_opengl2.h"
#include <string>

static bool show_demo_window = true;
static bool show_another_window = false;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

float m_fontScale = 0.5;
class GuiFont : public ImFont {
    ImFont* thiz;
public:
    GuiFont() {}

    void Use(std::string fontfile, float size) {
        thiz = ImGui::GetIO().Fonts->AddFontFromFileTTF(fontfile.c_str(), size * m_fontScale);
    }

    void Use(std::string font) {

    }

};
GuiFont imguiFont;

void ImGui_ImplAlgeSDK_Main(int x, int y, int guiScale) {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    //ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.DisplaySize.x = x * guiScale;
    io.DisplaySize.y = y * guiScale;

    m_fontScale = guiScale;
   // io.DisplayFramebufferScale = scale;
    
    //io.DisplayFramebufferScale = ImVec2(1, 1); //dpi opn 4k monitor

    // Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    ImGui::StyleColorsLight();
    //ImGui::SetWindowFontScale(2);
    ImGui_ImplGLUT_Init();
    
    if (io.BackendRendererUserData == nullptr)
        ImGui_ImplOpenGL2_Init();
}

void ImGui_ImplAlgeSDK_AfterRender()
{
    // Rendering
    ImGui::Render();
    //ImGuiIO& io = ImGui::GetIO();

     //   glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
     //   glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
     //  glClear(GL_COLOR_BUFFER_BIT);
    //glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound, but prefer using the GL3+ code.

    //  ImGui_ImplOpenGL2_RenderDrawData(...);
    
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
  
}


void ImGui_ImplAlgeSDK_BeforeRender()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGLUT_NewFrame();
    ImGui::NewFrame();
    ImGui::GetIO().FontGlobalScale = m_fontScale;
}

void ImGui_ImplAlgeSDK_Shutdown() {
    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGLUT_Shutdown();
    ImGui::DestroyContext();
}

void GuiStarts() {
    
    ImGui_ImplAlgeSDK_BeforeRender();
    
}

void GuiEnds() {
    ImGui_ImplAlgeSDK_AfterRender();
}


////////////////////////
