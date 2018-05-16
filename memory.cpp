#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <chrono>

using namespace std::chrono;
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
  int value;
  cpu_set_t set;
  start_timer();
  end_timer();
  start_timer();
  end_timer();
  sched_setaffinity(getpid(), sizeof(cpu_set_t), &set);
  for(int i = 0; i < 260; i++){
    uint64_t startt = start_timer();
    int * array = new int[(int)pow(2, 30)];
    //int * count = new int[5000];
    uint64_t endt = end_timer();
    //int index = 0;
    long double sum = 0;
    int iterations;
    int outer;
    /*if(i > 14){
      iterations = (int) pow(2, (30-i));
      outer = (int)pow(2,16)/iterations;
      //cout << "iterations: " << iterations << "outer: " << outer << endl;
    }*/
    //else{
      iterations = (int)pow(2.0,(double)i/10);
     //cout << iterations;
     if(i < 190){
     	outer = 1024*1024/iterations;
     }
     else{ outer = 2; }
    //}
    for(int k = 0; k < outer; k++){
      //cout << endt - startt << endl;
      //int * array = new int[(int)pow(2,30)];
      int index = 0;
      for(int j = 0; j < iterations; j++){
        int value;
       //int * array = new int[(int) pow(2,i)];
        uint64_t start = start_timer();
	//high_resolution_clock::time_point start = high_resolution_clock::now();
        value = array[index];
	//high_resolution_clock::time_point end = high_resolution_clock::now();
        uint64_t end = end_timer();
	if(k){
	  //duration<double> time_span = duration_cast<duration<double>>(end-start);
          //sum += time_span.count();
	  sum += (end - start);
	  //cout << end2 - start2 << " " << end - start << endl;
	  //cout << (end - start) - (end2 - start2) << endl;
	}
        /*else{
          cout << "time is: " << end - start << endl;
        }*/
        index += (int)pow(2, 4);
        //cout << "i: " << i << " and " << end - start << endl;
      
      
        //cout << "number of iterations is: " << j+1 << endl;
        //cout << "number of elements: " << pow(2,i) << endl;

        //cout << "index is: " << random << endl;
        //delete[] array;
      }
      //delete[] array;
    }
    long double average = (double)sum/((outer-1)*iterations);
    //average = (double)(average/2.3*1024*1024*1024);
    //cout << "sum is: " << sum << " ";
    //cout << "size of jump is: "<< (int)pow(2,i+2) << " and avg is: " << average << endl;
    //cout << "i: " << i << " average: " << average << endl;
    cout << average - 44 << endl;
    //cout << (int)pow(2,i) << "\t";
    delete[] array;
 }
  
  return 0;

}

