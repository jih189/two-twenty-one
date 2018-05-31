#include <iostream>
#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

int main(int argc, char * argv[]){
  int servSock;
  int clntSock;
  struct sockaddr_in servAddr;
  struct sockaddr_in clntAddr;
  unsigned int clntLen;
  int port = atoi(argv[1]);

  cout << "0" << endl;
  if((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
    perror("failed to create socket");
    return 1;
  }

  memset(&servAddr, 0, sizeof(servAddr));
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servAddr.sin_port = htons(port);

  if(bind(servSock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0){
    perror("failed to bind");
    return 1;
  }
  if(listen(servSock, 256) < 0){
    perror("failed to listen");
    return 1;
  }
  char * message = (char *) "2"; 
  char buffer[1024] = {0};
  bool breakLoop = false;
  int valread = 0;
  for(;;){
    clntLen = sizeof(clntAddr);
    cout << "1" << endl;
    if((clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntLen)) < 0){
      perror("failed to accept");
      break;
    }
    cout << "2" << endl;
    if((valread = read(clntSock, buffer, 1024)) < 0){
      perror("nothing to read");
      break;
    }
    cout << "here" << endl;
    while(valread > 0){
      send(clntSock, buffer, sizeof(buffer), 0);
      if((valread = read(clntSock, buffer, 1024)) < 0){
        perror("nothing to read");
        //breakLoop = true;
      }
    }
    cout << "kk" << endl;
    //close(clntSock);
  }

  //return 1;
}
