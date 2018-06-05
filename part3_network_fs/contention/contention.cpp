#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <cerrno>

//using namespace std::chrono;
using namespace std;

const int BLOCKSIZE=64; 


uint64_t startt, endt;
unsigned int high, low, id;

const size_t N = 10;
uint64_t durations[N];

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




double readfile_seq(const char *filename ){

    uint64_t sum = 0;
    uint64_t start, end;

    // open the file 
    int file = open(filename, O_DIRECT);
    // check for error
    if(file < 0){
        printf("Error opening file: %s\n", strerror(errno));
    }
    cout << "open file " << filename << endl;
    char* buf = (char *) malloc(BLOCKSIZE);
    size_t filesize = lseek(file, 0, SEEK_END);
    size_t blocknum = (size_t) filesize / BLOCKSIZE;
    lseek(file, 0, SEEK_SET);
    // read the file again
    for(int i = 0; i < blocknum; i++){
        start = start_timer();
        read(file, buf, BLOCKSIZE);
        end = end_timer();
        sum += (end - start);
    }
//     cout << "average reading time " << sum / blocknum << endl; 
    close(file);
    free(buf);
    return (double) sum/blocknum;
};

double readfile_rand(const char *filename){
     uint64_t sum = 0;
     uint64_t start, end;

     
     // open the file 
     int file = open(filename, O_DIRECT);
     // check for error
     if(file < 0){
          printf("Error opening file: %s\n", strerror(errno));
     }
     cout << "open file " << filename << endl;
     char* buf = (char *) malloc(BLOCKSIZE);
     size_t filesize;
     filesize = lseek(file, 0, SEEK_END);
     lseek(file, 0, SEEK_SET);
     size_t blocknum = filesize / BLOCKSIZE;
     srand(time(NULL));
     off_t accesspos;
     for(int i = 0; i < blocknum; i++){
          accesspos = (off_t) rand() % filesize;
          lseek(file, accesspos, SEEK_SET);
          start = start_timer();
          read(file, buf, BLOCKSIZE);
          end = end_timer();
          sum += (end - start);
     }
     // cout << "average reading time " << sum / blocknum << endl;
     close(file);
     free(buf);
     return (double)sum/blocknum;
};


int main(int argc, char * argv[]){

     double * ave_read_time = (double*) mmap(NULL, sizeof(double)*(argc-1)*N, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1 ,0 );     

     for(int i = 1; i < argc ; i++){
          pid_t pid = fork();
          if ( pid == 0) {
               for (int j = 0; j <N; j++) ave_read_time[(i-1)*N + j] = readfile_seq(argv[i]) ;
               _exit(0);
          }
     }
     for(int i = 1; i<argc ; i++){
          wait(NULL);
     }

     for(int i =0; i< (argc-1)*N ; i++){
          cout << ave_read_time[i] << endl;
     }


     return 0;
}