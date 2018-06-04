#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
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
  int value;
  cpu_set_t set;
  start_timer();
  end_timer();
  start_timer();
  end_timer();
  sched_setaffinity(getpid(), sizeof(cpu_set_t), &set);
  for(int i = 0; i < 250; i++){
    uint64_t startt = start_timer();
    int * array = new int[(int)pow(2, 30)];
    //int * count = new int[5000];
    uint64_t endt = end_timer();
    //int index = 0;
    long double sum = 0;
    int iterations;
    int outer;
    int div = 0;
    /*if(i > 14){
      iterations = (int) pow(2, (30-i));
      outer = (int)pow(2,16)/iterations;
    //cout << "iterations: " << iterations << "outer: " << outer << endl;
    }*/
    //else{
    outer = 1024;
    iterations = 2048;
    //acout << iterations;
    //}
    for(int k = 0; k < outer; k++){
      int index = 0;
      for(int j = 0; j < iterations; j++){
	int value;
	uint64_t start = start_timer();
	value = array[index];
	uint64_t end = end_timer();
	if(k){
	  sum += (end - start);
	  div++;
	}
	index += (int)pow(2, (double)i/10);
      }
    }
    long double average = (double)sum/div;
    //cout << div << endl;
    //average = (double)(average/2.3*1024*1024*1024);
    //cout << "sum is: " << sum << " ";
    //cout << "size of jump is: "<< (int)pow(2,i+2) << " and avg is: " << average << endl;
    //cout << "i: " << i << " average: " << average << " sum: " << sum << " divisor: " << div << "projected divisor: " << iterations*(outer-1) <<  endl;
    cout << average - 40 << endl;
    //cout << (int)pow(2,i) << "\t";
    delete[] array;
  }

  return 0;

}


