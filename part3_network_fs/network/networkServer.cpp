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
  int size = atoi(argv[2]);

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
  char * buffer = new char[1024*1024*1024];
  bool breakLoop = false;
  int valread = 0;
  for(;;){
    clntLen = sizeof(clntAddr);
    //cout << "1" << endl;
    if((clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntLen)) < 0){
      perror("failed to accept");
      break;
    }
    //cout << "2" << endl;
    if((valread = read(clntSock, buffer, size)) < 0){
      perror("nothing to read");
      break;
    }
    bool first = true;
    //cout << "here" << endl;
    int totalRead = 0;
    while(valread > 0){
      //cout << "before send" << endl;
      //cout << "valread " << valread << endl;
      //cout << size << "  "  << buffer[size-1] << endl;
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
      cout << "after send" << endl;
      //memset(buffer, 0, sizeof(buffer));
      if((valread = read(clntSock, buffer, size)) < 0){
        perror("nothing to read");
        //breakLoop = true;
      }
      //cout << "finished reading" << endl;
    }
    //cout << "kk" << endl;
    //close(clntSock);
  }
  cout << "ended loop" << endl;
  delete[] buffer;
  //return 1;
}
