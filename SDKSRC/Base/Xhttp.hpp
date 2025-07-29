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
		send(Socket, sz,(int) strlen(sz), 0);
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
#else
	void Close() {}
	int Recv(char* buffer, int len) { return 0; }
	void Send(char* sz) {}
	void Connect(string host_name) {
		_hostname = host_name;
//        sockfd = socket(AF_INET, SOCK_STREAM, 0);
//        if (sockfd < 0);
//       //   error("ERROR opening socket");
//        bzero((char *) &serv_addr, sizeof(serv_addr));
//        portnom = 80;
        
	}


#endif // WIN32

};
#ifndef MAX_PATH
#define MAX_PATH 260
#endif

extern char ResPath[MAX_PATH];
extern XHttpSocket msck;

//transferred to XCOMMONS
std::vector<std::string> split(const std::string& s, char seperator)
{
	std::vector<std::string> output; std::string::size_type prev_pos = 0, pos = 0; while ((pos = s.find(seperator, pos)) != std::string::npos) { std::string substring(s.substr(prev_pos, pos - prev_pos)); output.push_back(substring); prev_pos = ++pos; } output.push_back(s.substr(prev_pos, pos - prev_pos));
	return output;
}


// if buf is passed we will not allocate internally
string getHttpFile(string hostname, string resourcepath, string filename,bool returnfilename = false, int* nbytes = nullptr, char* buf = nullptr)
{
    string fileAndRes = filename;//(resourcepath + "/" + filename);
	std::replace(fileAndRes.begin(), fileAndRes.end(), '/', '_');
	string localfilename = (string(ResPath) + fileAndRes);
	FILE* f = fopen(localfilename.c_str(), "r");
	if (f) {
		fseek(f, 0, SEEK_END);
		int size = (int)ftell(f);
		fclose(f);
		if (size != 0) //recollect file if prev retrival was erroneous
			if (returnfilename) return localfilename;//Dont proceed if file already exists
	}
	if (buf == nullptr) f = fopen((ResPath + filename).c_str(), "wb");

	msck.Connect(hostname);

	string get1 = "GET /" + resourcepath + "/" + filename + " HTTP/1.1\r\nHost: " + hostname + " \r\nConnection: close\r\n\r\n";
	const char* sz = get1.c_str();
	msck.Send((char*)sz);
	int len = 1024 * 1024 + 10;
	char* locallyallocatedbuffer = nullptr;
	
	char* buffer = nullptr;
	if (buf != nullptr) buffer = buf; else {
		locallyallocatedbuffer = (char*)malloc(len);
		buffer = locallyallocatedbuffer;
	}
	// if buf is passed we will not allocate internally
	
	if (buffer == nullptr) {
		return "buffer-issue";
	}

	int r1 = msck.Recv(buffer, len);

	try {
		char* ll = strstr(buffer, "ength:");
		if (!ll) return "";
		len = atoi(ll + 7);
		//int diff = r1 - len;
	}
	catch (...) {
		return "";
	}

	int bytestogo = len;
	char* eob = buffer + (1024 * 1024);
	char* wherecontentbegins = eob - bytestogo;
	char* buffptr = wherecontentbegins;
	int totalbytesread = 0;

	if (r1 > len) {//all bytes have been read in first block
		bytestogo = 0;
		totalbytesread = len;
		wherecontentbegins = buffer + (r1 - len);
	}

	while (bytestogo > 0) {
		int bytesread = msck.Recv(buffptr, bytestogo);
		if (bytesread > 0) {
			bytestogo -= bytesread;
			buffptr += bytesread;
			totalbytesread += bytesread;
		}
		else break;
	}

	int lostbytes = len - totalbytesread;
	wherecontentbegins[totalbytesread] = 0;
	char* contents = "";
	if (lostbytes == 0) {
		if (buf == nullptr)
		{
			if (f) fwrite(wherecontentbegins, len, 1, f);
			contents = (wherecontentbegins);
			if (locallyallocatedbuffer !=nullptr)
					free(locallyallocatedbuffer);//buffer is allocated only when buf == nulptr
		}
	}

	msck.Close();
	if (f) fclose(f);
	if (nbytes != nullptr) *nbytes = len;
	return (returnfilename?string(filename):contents);
}

string GetHttpsData(string hostname, string resourcepath, string filename, XHttpSocket& msck) {
    
    msck.Connect(hostname);

    string get1 = "GET " + resourcepath + "" + filename + " HTTP/1.1\r\nHost: " + hostname + " \r\nConnection: close\r\n\r\n";
    const char* sz = get1.c_str();
    msck.Send((char*)sz);
    int len = 40969;
    char* buffer = (char*)malloc(len);// if buf is passed we will not allocate internally
    int r1 = msck.Recv(buffer, len);

    try {
        char* ll = strstr(buffer, "Length:");
		if (!ll) {
			free(buffer); return "";
		}
        len = atoi(ll + 8);
        //int diff = r1 - len;
    }
    catch (...) {
		free(buffer);
		return "";
    }

    int bytestogo = len;
    char* eob = buffer + (1024 * 1024);
    char* wherecontentbegins = eob - bytestogo;
    char* buffptr = wherecontentbegins;
    int totalbytesread = 0;

    if (r1 > len) {//all bytes have been read in first block
        bytestogo = 0;
        totalbytesread = len;
        wherecontentbegins = buffer + (r1 - len);
    }

    while (bytestogo > 0) {
        int bytesread = msck.Recv(buffptr, bytestogo);
        if (bytesread > 0) {
            bytestogo -= bytesread;
            buffptr += bytesread;
            totalbytesread += bytesread;
        }
        else break;
    }

    int lostbytes = len - totalbytesread;
	free(buffer);
	return string(wherecontentbegins);
}

string GetHttp(string url, bool returnfilename) {
	string remote = url;
	vector<string> strr = split(remote, '/');
	int n = (int)strr.size();
	string hostname = strr[2];
	string resourcepath = "";
	for (int i = 3; i < n - 1; i++) {
		resourcepath += (i == 3 ? "" : "/") + strr[i];
	}
	string fname = strr[n - 1];
	return getHttpFile(hostname, resourcepath, fname,returnfilename);
	//	curlpp::Easy request;
}

string retrieve(char** url, bool returnfilename) { 
	return GetHttp(string(*url), returnfilename);
}

#else

#endif // !Xhttp
