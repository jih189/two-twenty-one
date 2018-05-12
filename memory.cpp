#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

using namespace std;

uint64_t startt, endt, duration;
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
  int value;
  for(int i = 2; i < 31; i++){
    uint64_t startt = start_timer();
    int * array = new int[(int)pow(2, i)];
    uint64_t endt = end_timer();
    cout << endt - startt << endl;
    for(int j = 0; j < 100000; j++){
      int random = rand() % (int)pow(2,i);
      uint64_t start = start_timer();
      value = array[random];
      uint64_t end = end_timer();
      
      //cout << "number of iterations is: " << j+1 << endl;
      //cout << "number of elements: " << pow(2,i) << endl;
      cout << end - start << endl;
      //cout << "index is: " << random << endl;
      
    }
    delete[] array;
  }
  
  return 0;

}

