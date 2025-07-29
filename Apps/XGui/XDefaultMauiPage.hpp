
namespace AppLogic {

    
    class Logic {
        static int click_count;
    public:
        
        static string onUserAction(string text) {
            string ret = getHttpFile("icetop.acnodelabs.com", "/icetop.php?proj=IsbVeh&suburl=/Vehicle", text+ ".json");
            return ret;
        }
    };
       

    //The Bridge is the interface to the AlgeApp
    class Bridge {
    public:
        static Logic Logic;
        static string onBtn(int i, string text) {
            return Logic.onUserAction(text);
        }
    };
}

