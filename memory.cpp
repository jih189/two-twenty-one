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
  for(int i = 0; i < 300; i++){
    uint64_t startt = start_timer();
    //int * array = new int[(int)pow(2, 30)];
    //int * count = new int[5000];
    uint64_t endt = end_timer();
    //int index = 0;
    long sum = 0;
    int iterations;
    int outer;
    if(i > 100){
      iterations = (int) pow(2, (30-i/10));
      outer = (int)pow(2,20)/iterations;
      //cout << "iterations: " << iterations << "outer: " << outer << endl;
    }
    else{
      iterations = (int)pow(2,20);
      outer = 1;
    }
    for(int k = 0; k < outer; k++){
      //cout << endt - startt << endl;
      int * array = new int[(int)pow(2,30)];
      int index = 0;
      for(int j = 0; j < iterations; j++){
        int value;
        //int * array = new int[(int) pow(2,i)];
        uint64_t start = start_timer();
        value = array[index];
        uint64_t end = end_timer();
        if(j){
          sum += (end - start);
        }
        /*else{
          cout << "time is: " << end - start << endl;
        }*/
        index += (int)pow(2, i/10);
        //cout << "i: " << i << " and " << end - start << endl;
      
      
        //cout << "number of iterations is: " << j+1 << endl;
        //cout << "number of elements: " << pow(2,i) << endl;

        //cout << "index is: " << random << endl;
        //delete[] array;
      }
      delete[] array;
    }
    int average = sum/(outer *(iterations-1));
    //cout << "sum is: " << sum << " ";
    //cout << "size of jump is: "<< (int)pow(2,i+2) << " and avg is: " << average << endl;
    cout << average << endl;
    //cout << (int)pow(2,i) << "\t";
    //delete[] array;
 }
  
  return 0;

}

