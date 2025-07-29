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
