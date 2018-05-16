#include <stdint.h>
#include <iostream>
#include <time.h>
#include <sched.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <math.h>

using namespace std;
unsigned int high, low, id;
#define L3CACHSIZE 4194304 
static inline uint64_t start_timer(){
    __asm__ volatile( "cpuid \n\t"
        "rdtsc \n\t"
        "mov %%edx, %0 \n\t"
        "mov %%eax, %1 \n\t"
        : "=r"(high), "=r"(low)
        :: "%rax", "%rbx", "%rcx", "%rdx"
    );
    return ((uint64_t)high <<32) | low;
}
static inline uint64_t end_timer(){
    __asm__ volatile( 
        "rdtscp \n\t"
        "mov %%edx, %0 \n\t"
        "mov %%eax, %1 \n\t"
        "cpuid \n\t"
        : "=r"(high), "=r"(low)
        :: "%rax", "%rbx", "rcx", "%rdx"
    );
    return ((uint64_t)high <<32) | low;
}

int main(int argc, char *argv[]){
    uint64_t offset = 16;
    // create a memory on heap
//    if(argv[1][0] == '0'){
    int *mem1 = new int[(int)pow(2, 30)];
    int index = 0;
    for(int i = 0; i < (int)pow(2,24); i++){
       mem1[index] = 0;
       index += 16;
    }
    uint64_t start1, end1;
    start1 = start_timer();
    mem1[0] = 6;
    mem1[offset] = 6;
    mem1[2*offset] = 6;
    mem1[3*offset] = 6;
    mem1[4*offset] = 6;
    mem1[5*offset] = 6;
    mem1[6*offset] = 6;
    mem1[7*offset] = 6;
    mem1[8*offset] = 6;
    mem1[9*offset] = 6;
    mem1[10*offset] = 6;
    mem1[11*offset] = 6;
    mem1[12*offset] = 6;
    mem1[13*offset] = 6;
    mem1[14*offset] = 6;
    mem1[15*offset] = 6;
    mem1[16*offset] = 6;
    mem1[17*offset] = 6;
    mem1[18*offset] = 6;
    mem1[19*offset] = 6;
    mem1[20*offset] = 6;
    mem1[21*offset] = 6;
    mem1[22*offset] = 6;
    mem1[23*offset] = 6;
    mem1[24*offset] = 6;
    mem1[25*offset] = 6;
    mem1[26*offset] = 6;
    mem1[27*offset] = 6;
    mem1[28*offset] = 6;
    mem1[29*offset] = 6;
    mem1[30*offset] = 6;
    mem1[31*offset] = 6;
    mem1[32*offset] = 6;
    mem1[33*offset] = 6;
    mem1[34*offset] = 6;
    mem1[35*offset] = 6;
    mem1[36*offset] = 6;
    mem1[37*offset] = 6;
    mem1[38*offset] = 6;
    mem1[39*offset] = 6;
    mem1[40*offset] = 6;
    mem1[41*offset] = 6;
    mem1[42*offset] = 6;
    mem1[43*offset] = 6;
    mem1[44*offset] = 6;
    mem1[45*offset] = 6;
    mem1[46*offset] = 6;
    mem1[47*offset] = 6;
    mem1[48*offset] = 6;
    mem1[49*offset] = 6;
    end1 = end_timer();
    double cycles = (double)(end1 - start1)/50.0;
    double seconds = cycles/(2.3*1024*1024*1024);
    double bandwidth = 50.0/seconds;
    std::cout << "writing bandwidth is: " << bandwidth << std::endl;
    std::cout << "overhead of writing to main memory : " << (end1 - start1) / 50 << std::endl; 
    delete mem1;
//    }else{
    uint64_t start, end;
    int in;
    int *mem = new int[(int)pow(2, 30)];
    index = 0;
    for(int i = 0; i < (int)pow(2,26); i++){
       mem[index] = 0;
       index += 16;
    } 
    start = start_timer();
    in = mem[0+1];
    in = mem[offset+1];
    in = mem[2*offset+1];
    in = mem[3*offset+1];
    in = mem[4*offset+1];
    in = mem[5*offset+1];
    in = mem[6*offset+1];
    in = mem[7*offset+1];
    in = mem[8*offset+1];
    in = mem[9*offset+1];
    in = mem[10*offset+1];
    in = mem[11*offset+1];
    in = mem[12*offset+1];
    in = mem[13*offset+1];
    in = mem[14*offset+1];
    in = mem[15*offset+1];
    in = mem[16*offset+1];
    in = mem[17*offset+1];
    in = mem[18*offset+1];
    in = mem[19*offset+1];
    in = mem[20*offset+1];
    in = mem[21*offset+1];
    in = mem[22*offset+1];
    in = mem[23*offset+1];
    in = mem[24*offset+1];
    in = mem[25*offset+1];
    in = mem[26*offset+1];
    in = mem[27*offset+1];
    in = mem[28*offset+1];
    in = mem[29*offset+1];
    in = mem[30*offset+1];
    in = mem[31*offset+1];
    in = mem[32*offset+1];
    in = mem[33*offset+1];
    in = mem[34*offset+1];
    in = mem[35*offset+1];
    in = mem[36*offset+1];
    in = mem[37*offset+1];
    in = mem[38*offset+1];
    in = mem[39*offset+1];
    in = mem[40*offset+1];
    in = mem[41*offset+1];
    in = mem[42*offset+1];
    in = mem[43*offset+1];
    in = mem[44*offset+1];
    in = mem[45*offset+1];
    in = mem[46*offset+1];
    in = mem[47*offset+1];
    in = mem[48*offset+1];
    in = mem[49*offset+1];
    end = end_timer();
    cycles = (double)(end-start)/50.0;
    seconds = (double)cycles/(2.3*1024*1024*1024);
    bandwidth = 50.0/seconds;
    std::cout << "reading bandwidth is: " << bandwidth << std::endl;
    std::cout << "overhead of reading from main memory : " << (end - start) / 50 << std::endl; 
    delete mem;
//    }
    return 0; 
}
