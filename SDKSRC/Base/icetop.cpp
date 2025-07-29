
//#ref : https://www.linuxhowtos.org/data/6/client.c

#ifndef WIN32
#include "nonwinsock.c"

class IceTop {
public:

    IceTop() {}
    
    std::string unset = "!unset!";
    std::string proj = unset;
    bool verbose = false;
    std::string server = "icetop.acnodelabs.com";
    std::string lasturl = unset;

    IceTop(string project) {
        proj = project;
    }

private:
    string _geturl(string suburl) {
        return "https://" + server + "/icetop.php?proj=" + proj + "&suburl=" + suburl;
    }

public:

    string Get(std::string suburl) {
        std::string url = _geturl(suburl);
        char* argv[] = { (char*)"path", (char*)server.c_str(), (char*)"80" };
        string ret = nonwinsock(3, argv, "icetop.php?proj=" + proj + "&suburl=" + suburl);
        lasturl = url;
        return ret;
    }

    void Put(std::string suburl, std::string data) {
        std::string url = _geturl(suburl);
        lasturl = url;
    }
};
#else
#include "Xhttp.hpp"
//Windows version
#include <string>
using namespace std;
class IceTop {
public:

    std::string unset = "!unset!";
    std::string proj = unset;
    bool verbose = false;
    std::string server = "icetop.acnodelabs.com";
    std::string lasturl = unset;

private:
    string _geturl(string suburl) {
        return "https://" + server + "/icetop.php?proj=" + proj + "&suburl=" + suburl;
    }

public:

    IceTop() {
    
    }
    IceTop(string prj) {
        proj = prj;
    }

    string Get(std::string suburl) {
        
        std::string url = _geturl(suburl);
        XHttpSocket isck;
        string hostname = "icetop.acnodelabs.com";
        string resourcepath = "/";
        string filename = "icetop.php?proj=" + proj + "&suburl=" + suburl;
        isck.Connect(hostname);

        string get1 = "GET " + resourcepath + "" + filename + " HTTP/1.1\r\nHost: " + hostname + " \r\nConnection: close\r\n\r\n";
        const char* sz = get1.c_str();
        isck.Send((char*)sz);
        int len = 40969;
        char* buffer = (char*)malloc(len);// if buf is passed we will not allocate internally
        memset(buffer, 0, len);
        int r1 = isck.Recv(buffer, len);

        try {
            char* ll = strstr(buffer, "content-length:");
            if (!ll) {
                isck.Close();
                free(buffer); return "";
            }
            len = atoi(ll + 15);
            //int diff = r1 - len;
        }
        catch (...) {
            isck.Close();
            free(buffer);
            return "";
        }
        lasturl = url;
        char* payload = strstr(buffer, "\r\n\r\n");
        string ret = string(payload+4);
        free(buffer);
        return ret;
    }

    bool Put(std::string suburl, std::string data) {
        std::string url = _geturl(suburl);
        lasturl = url;
        return true;
    }
};
#endif

