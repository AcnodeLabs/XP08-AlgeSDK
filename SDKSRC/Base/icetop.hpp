
//#ref : https://www.linuxhowtos.org/data/6/client.c
#define WIN32
#ifndef WIN32

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <iostream>
using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

char* getpayload(char* str) {
    char delim[] = "\n\n";
    char *token = strtok(str,delim);
    if (!token) return str;
    int n = 0;
    int content_len = 0;
      // loop until strtok() returns NULL
      while (token)  {

        // print token
       cout << token << endl;
       
        // take subsequent tokens
        token = strtok(NULL,delim);
          if (string(token).find("content-length:")!=std::string::npos) {
              content_len = atoi(token+15);
              if (content_len>0) {
                char* payload = str + 152;
                  //pick slightly ahead so that if contentlen digits are less then we dont miss the data
                  //safely move fwd if required
                  if (*payload=='\r') payload ++;
                  if (*payload=='\n') payload ++;
                  if (*payload=='\r') payload ++;
                  if (*payload=='\n') payload ++;
                return  payload;
              }
          }
          if (n==7 && str[9]=='4' && str[10]=='0' && str[11]=='4')
              return str+13;
          else if (n==7)
              return token;
          n++;
      }
    return str;
}
char buffer[40969];
string nonwinsock(int argc, char *argv[], string what)
{
    int sockfd, portno;
    long n;//chnage to int if this function is not working as expected
    struct sockaddr_in serv_addr;
    struct hostent *server;

    
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    printf("Please enter the message: ");
    bzero(buffer,4096);
  //  fgets(buffer,255,stdin);
    string get1 = "GET /" + what + " HTTP/1.1\r\nHost: " + string(argv[1]) + " \r\nConnection: close\r\n\r\n";
    const char* buffe = get1.c_str();
    n = write(sockfd,buffe,strlen(buffe));
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,4096);
    n = read(sockfd,buffer,4096);
    if (n < 0) 
         error("ERROR reading from socket");
    
    close(sockfd);
    return string(getpayload(buffer));
}

class IceTop {
public:

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
        char* argv[] = { (char*)"path", (char*)"icetop.acnodelabs.com", (char*)"80" };
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
#ifndef XHTTP_INCLUDED
#define XHTTP_INCLUDED
#ifndef _WINDOWS_
//#include <Windows.h>
#endif
#ifndef WIN32
#include <sys/types.h>
#include <sys/socket.h>
#endif
#define Xhttp
using namespace std;
class XHttpSocket {
	string _hostname;

public:

#ifdef WIN32
	WSADATA wsaData;
	SOCKADDR_IN SockAddr;
	SOCKET Socket;

	void Connect(string host_name) {
		_hostname = host_name;

		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			cout << "WSAStartup failed.\n";
			return;
		}
		Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		struct hostent* host;

		host = gethostbyname(_hostname.c_str());

		SockAddr.sin_port = htons(80);
		SockAddr.sin_family = AF_INET;
		SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);
		std::cout << "Connecting...\n";
		if (connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0) {
			cout << "Could not connect";
			return;
		}
		cout << "Connected.\n";

	}


	void Close() {
		closesocket(Socket);
		WSACleanup();
	}

	int Recv(char* buffer, int len) {
		return recv(Socket, buffer, len, 0);
	}

	void Send(char* sz) {
		send(Socket, sz, strlen(sz), 0);
	}


    struct sockaddr_in
    {
      short   sin_family; /* must be AF_INET */
      u_short sin_port;
      struct  in_addr sin_addr;
      char    sin_zero[8]; /* Not used, must be zero */
    };
    struct sockaddr_in serv_addr, cli_addr;

    int sockfd, newsockfdm,portnom,clilen, n;
    
	//void Close() {}
	//int Recv(char* buffer, int len) { return 0; }
	//void Send(char* sz) {}
	//void Connect(string host_name) {
	//	_hostname = host_name;
 //       sockfd = socket(AF_INET, SOCK_STREAM, 0);
 //       if (sockfd < 0);
 //      //   error("ERROR opening socket");
 //       bzero((char *) &serv_addr, sizeof(serv_addr));
 //       portnom = 80;
 //       
	//}

#endif // WIN32

};

string GetHttp(string url) {
	string remote = url;
	vector<string> strr = split(remote, '/');
	int n = (int)strr.size();
	string hostname = strr[2];
	string resourcepath = "";
	for (int i = 3; i < n - 1; i++) {
		resourcepath += (i == 3 ? "" : "/") + strr[i];
	}
	string fname = strr[n - 1];
	return getHttpFile(hostname, resourcepath, fname);
	//	curlpp::Easy request;
}

string retrieve(char** url) { 
	return GetHttp(string(*url));
}

#else

#endif // !Xhttp

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

    void Put(std::string suburl, std::string data) {
        std::string url = _geturl(suburl);
        lasturl = url;
    }
};
#endif

