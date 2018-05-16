#include <stdint.h>
#include <iostream>
#include <time.h>
#include <sched.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <thread>

using namespace std;
unsigned int high, low, id;
uint64_t starttime;

static inline uint64_t start_timer(){
    __asm__ volatile ( "cpuid \n\t"
         "rdtsc \n\t"
         "mov %%edx, %0 \n\t"
         "mov %%eax, %1 \n\t"
         : "=r"(high), "=r"(low)
         :: "%rax", "%rbx", "%rcx", "%rdx"
     );
    return ((uint64_t)high << 32)|low;
}


static inline uint64_t end_timer(){
    __asm__ volatile ( 
         "rdtscp \n\t"
         "mov %%edx, %0 \n\t"
         "mov %%eax, %1 \n\t"
         "cpuid \n\t"
         : "=r"(high), "=r"(low)
         :: "%rax", "%rbx", "%rcx", "%rdx"
     );
    return ((uint64_t)high << 32)|low;
}

void* foo(void*) {
    uint64_t end = end_timer();
    if(starttime != 3 && end > starttime){
        cout << end - starttime << endl;
    }else if(starttime != 3){
        cout << starttime - end << endl;
    }
    pthread_exit(NULL);
}
void* foo1(void*) {
    uint64_t end = end_timer();
    cout << end - starttime << endl;
    pthread_exit(NULL);
}

uint64_t p1,p2;
int main(int argc, char **argv){
/* thread context switch
    pthread_t td1;
    starttime = 3;
    pthread_create(&td1, NULL, foo, NULL);
    starttime = start_timer();
    pthread_join(td1, NULL);
  */  
/*create thread
    pthread_t td1;
    starttime = start_timer();
    pthread_create(&td1, NULL, foo1, NULL);
    pthread_join(td1, NULL);
*/
//create process
/*
    uint64_t start, end1, end2;
    start = start_timer();
    pid_t pid = fork();
    if(pid == 0){
        // child process
        end1 = end_timer();
        cout << (end1 - start) << endl;
   
    }
    else if(pid > 0){
        end2 = end_timer();
        cout << (end2 - start) << endl;
        // parent process
    }
*/
// processes switch
    uint64_t s, e, overheadOfPrint;

    s = start_timer();
    cout << 'x' << 7654321-1234567 << endl;
    e = end_timer();
    overheadOfPrint = e - s;

    starttime = start_timer();
    pid_t pid = fork();
    if(pid == 0){
        // child process
        cout << 'c' << end_timer() - starttime << endl; 
        cout << 'c' << end_timer() - starttime - overheadOfPrint << endl; 
        cout << 'c' << end_timer() - starttime - 2*overheadOfPrint << endl; 
    }
    else if(pid > 0){
        // parent process
        cout << 'p' << end_timer() - starttime << endl; 
        cout << 'p' << end_timer() - starttime - overheadOfPrint << endl; 
        cout << 'p' << end_timer() - starttime - 2*overheadOfPrint << endl; 
    }

    return 0;

}
