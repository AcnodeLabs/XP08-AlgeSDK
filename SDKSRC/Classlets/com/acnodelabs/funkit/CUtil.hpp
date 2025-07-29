#ifdef CUTIL_INCLUDED
#else
#define CUTIL_INCLUDED
#include <sstream>
#include <string>
using namespace std;

class CUtil {
public:
    static string stripChar(string s, char x = '0') {
        int unwanted_from_start = 0;
        int unwanted_from_end = 0;

        for (size_t i = 0; i < s.length(); i++ ) {
            if (s[i] == x) unwanted_from_start++; else break;
        }
        s.erase(0, unwanted_from_start);

        for (int i = int(s.length()); i >= 0; i--) {
            if (s[i] == x) unwanted_from_end++; else continue;
        }
        s = s.substr(0, s.length() - unwanted_from_end -1);

        return s;
    }

    static float stdStringToFloat(std::string name) {
            std::string num = name;
            double temp = ::atof(num.c_str());
            return (float(temp));
    }
    
    static std::string floatToStdString(float f) {
            std::ostringstream ss;
            ss << f;
            std::string s(ss.str());
            return s;
    }
};
#endif
