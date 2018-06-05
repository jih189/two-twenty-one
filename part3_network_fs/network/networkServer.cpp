#include <iostream>
#include <stdio.h>     
#include <sys/socket.h> 
#include <arpa/inet.h>  
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
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
  int size = atoi(argv[2]);

  if((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
    perror("failed to create socket");
    return 1;
  }

  memset(&servAddr, 0, sizeof(servAddr));
  servAddr.sin_family = AF_INET;
  servAddr.sin_port = htons(port);
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

  if(bind(servSock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0){
    perror("failed to bind");
    return 1;
  }
  if(listen(servSock, 256) < 0){
    perror("failed to listen");
    return 1;
  }
  char * message = (char *) "2"; 
  char * buffer = new char[1024*1024*1024];
  bool breakLoop = false;
  int valread = 0;
  for(;;){
    clntLen = sizeof(clntAddr);
    
    if((clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntLen)) < 0){
      perror("failed to accept");
      break;
    }
    
    if((valread = read(clntSock, buffer, size)) < 0){
      perror("nothing to read");
      break;
    }
    bool first = true;
    int totalRead = 0;
    while(valread > 0){
      if(first){
        first = false;
	send(clntSock,buffer, 1, 0);
      }
      else{
      	totalRead += valread;
      	while(totalRead >= size){
          send(clntSock, buffer, size, 0);
          totalRead -= size;
        }
      }
      if((valread = read(clntSock, buffer, size)) < 0){
        perror("nothing to read");
      }
    }
  }
  cout << "ended loop" << endl;
  delete[] buffer;
}
