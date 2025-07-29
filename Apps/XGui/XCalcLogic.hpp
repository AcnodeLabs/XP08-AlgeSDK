#ifdef  WIN32
#else
#include "../../../XP08-AlgeSDK XCOMMONS/cpp/reuse/programs/MauiCalc/logic.cpp"
#endif //  WIN32



typedef int(__cdecl* call)(char* appname, char* function);
typedef int(__cdecl* call_s)(char* appname, char* function, char* buf);

namespace AppLogic {

    namespace MauiCalc {
        DLL Logic("xcommons.dll");
    }

    class Bridge {
    public:
        
        static void Init() { int ret = ((call)MauiCalc::Logic.Fn("call"))("MauiCalc", "Init");}
        
        //static MauiCalc::Logic Logic;
        static string onBtn(int i) {
          char ItemText[32];
          strcpy(ItemText, to_string(i).c_str());
          ((call_s)MauiCalc::Logic.Fn("call_s"))("MauiCalc", "OnClick", ItemText);
          ItemText[0] = 0;
          int ret = ((call_s)MauiCalc::Logic.Fn("call_s"))("MauiCalc", "getAnswer", ItemText);
          return string(ItemText);
        }

        static void Init(AlgeApp* that) {
           that->SetTitle("XCalc");
           Init();
        }

    };
}

