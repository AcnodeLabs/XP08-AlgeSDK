#include <string>
using namespace std;

// XGui stuff
#include "../../../imgui/imgui.h"
#include "../../../imgui/imgui_internal.h"
#include "../../../imgui/backends/imgui_impl_win32.h"
#include "../../../imgui/backends/imgui_impl_opengl2.h"

static bool show_demo_window = true;
static bool show_another_window = false;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

float m_fontScale = 0.5;

//Fix TODO:: If app is to be launched as 'open with', ensure C:\Windows\System32\ contains OpenSans-Regular.ttf file to make AddFontFromFileTTF

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

void ImGui_ImplAlgeSDK_Main(int x, int y, float guiScale) {
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	//guiFont.Init("Roboto-Bold.ttf", 10);
	ImGui::CreateContext();
	
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.DisplaySize.x = x * guiScale;
	io.DisplaySize.y = y * guiScale;
	m_fontScale = guiScale;
	io.FontGlobalScale = m_fontScale;
	ImGui::StyleColorsLight();

}
static char msxg[1024];
void ImGui_ImplAlgeSDK_AfterRender()
{
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

void ImGui_ImplAlgeSDK_BeforeRender()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::GetIO().FontGlobalScale = m_fontScale;
}

void ImGui_ImplAlgeSDK_Shutdown() {
	// Cleanup
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void GuiStarts() {
//	ImGui_ImplAlgeSDK_BeforeRender();
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplWin32_NewFrame();
}

void GuiEnds() {
	ImGui_ImplAlgeSDK_AfterRender();
}


#include "imgui_internal.h"

int rotation_start_index;
void ImRotateStart()
{
	rotation_start_index = ImGui::GetWindowDrawList()->VtxBuffer.Size;
}

ImVec2 ImRotationCenter()
{
	ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX); // bounds

	const auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
	for (int i = rotation_start_index; i < buf.Size; i++)
		l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos);

	return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2); // or use _ClipRectStack?
}

//ImVec2 operator-(const ImVec2& l, const ImVec2& r) { return{ l.x - r.x, l.y - r.y }; }

void ImRotateEnd(float rad, ImVec2 center = ImRotationCenter())
{
	double s = sin(rad), c = cos(rad);
	center = ImRotate(center, (float)s, (float)c) - center;

	auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
	for (int i = rotation_start_index; i < buf.Size; i++)
		buf[i].pos = ImRotate(buf[i].pos, (float)s, (float)c) - center;
}



////////////////////////