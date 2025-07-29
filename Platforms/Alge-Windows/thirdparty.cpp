//these cpp files are required in compilation
// they are put in this file thirdparty.hpp 
// to simplify project structure

//Timer.ccp/.h : Copyright (c) 2003 Song Ho Ahn
#include "Timer.cpp"

#include "..\..\SDKSRC\Base\camera.cpp"

//jpgd.ccp/.h : Rich Geldreich <richgel99@gmail.com>
#include "..\..\SDKSRC\Base\jpgd.cpp"


///local copy of https://github.com/ocornut/imgui 
#include "..\..\..\imgui\imgui.cpp"
#include "..\..\..\imgui\imgui_demo.cpp"
#include "..\..\..\imgui\imgui_draw.cpp"
#include "..\..\..\imgui\imgui_tables.cpp"
#include "..\..\..\imgui\imgui_widgets.cpp"
#include "..\..\..\imgui\backends\imgui_impl_opengl2.cpp"
#include "..\..\..\imgui\backends\imgui_impl_win32.cpp"

#include "../../SDKSRC/Base/fmod/framework.hpp"


// Use AlgeSDK/setup_deps script to get external repos in your sandbox