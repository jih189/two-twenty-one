#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>      
#include <sys/socket.h> 
#include <arpa/inet.h>  
#include <string.h> 
#include <sys/stat.h>
//#include <chrono>

//using namespace std::chrono;
using namespace std;

uint64_t startt, endt;
unsigned int high, low, id;

static inline uint64_t start_timer(){
  __asm__ volatile( "cpuid \n\t"
      "rdtsc \n\t"
      "mov %%edx, %0 \n\t"
      "mov %%eax, %1 \n\t"
      : "=r"(high), "=r"(low)
      :: "%rax", "%rbx", "%rcx", "%rdx");
  return ((uint64_t)high << 32) | low;
}


static inline uint64_t end_timer(){
  __asm__ volatile( "rdtscp \n\t"
      "mov %%edx, %0 \n\t"
      "mov %%eax, %1 \n\t"
      "cpuid \n\t"
      : "=r"(high), "=r"(low)
      :: "%rax", "%rbx", "%rcx", "%rdx");
  return ((uint64_t)high << 32) | low;
}

int main(int argc, char * argv[]){
  struct sockaddr_in clntAddr;
  struct sockaddr_in servAddr;
  int servSock;
  int clntSock;
  char * message = (char *)"1";
  char buffer[1024] = {0};
  uint64_t totalRT = 0;
  uint64_t totalSetup = 0;
  uint64_t totalTear = 0;
  double totalBand = 0;
  double totalBandRec = 0;

  for(int i = 0; i < 100; i++){
    if((clntSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
      cout <<"socket() failed" << endl;
      exit(1);
    }

    memset(&servAddr, 0, sizeof(clntAddr));
    int port = atoi(argv[1]);
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(port);

    if(inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr) <= 0){
      cout << "conversion failed" << endl;
      return 1;
    }
    
    uint64_t startSetup = start_timer();
    connect(clntSock, (struct sockaddr *)&servAddr, sizeof(servAddr));
    uint64_t endSetup = end_timer();
    totalSetup += (endSetup - startSetup);

    uint64_t start = start_timer();
    send(clntSock, message, strlen(message), 0);
    int valread = read(clntSock, buffer, 1024);
    uint64_t end = end_timer();

    totalRT += (end - start);

    string message2 = "";
    for(int i = 0; i < 1024; i++){
      message2 += 'a';
    }
    const char * message3 = message2.c_str();
    uint64_t startBand = start_timer();
    send(clntSock, message3, strlen(message3), 0);
    valread = read(clntSock, buffer, 1024);
    uint64_t endBand = end_timer();

    double bandRec = (double)(endBand - startBand)/1024;
    totalBandRec += bandRec;
    
    uint64_t startTear = start_timer();
    close(clntSock);
    uint64_t endTear = end_timer();
    totalTear += (endTear - startTear);
  }
  cout << "Average bandwidth is: " << (double)100/(totalBandRec) << endl;
  cout << "Average setup time is: " << (double)totalSetup/100 << endl;
  cout << "Average teardown time is: " << (double)totalTear/100 << endl;
  cout << "Average round-trip is: " << (double)totalRT/100 << endl;
}
