#include <stdint.h> /* for uint64_t */
#include <time.h>  /* for struct timespec */
#include <unistd.h> /* for fork */
#include <pthread.h> /* for pthread */
#include <sched.h>

#include <iostream>

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>       
#include <sys/types.h>


#include <vector>
#include <limits>
#include <stdio.h>
#include <string.h>


using namespace std;


// global variable, record the start and end CPU time stampe
uint64_t startt, endt, duration;
unsigned int high, low, id;
//unsigned long flags;
//struct sched_param params;
const size_t N = 1000;
uint64_t durations[N];



/* assembly code to read the cpu count at time stamp */
static inline uint64_t cpu_cycle_RDTSC() 
{
     __asm__ volatile( "rdtsc" :  "=a" (low), "=d" (high)); 
     // This is the assemble instruction that saves the cpu cycle count

     return ((uint64_t)high << 32) | low ;
}


static inline uint64_t start_timer(){
     //preempt_disable();
     //raw_local_irq_save(flags); 
     __asm__ volatile( "cpuid \n\t"
          "rdtsc \n\t"
          "mov %%edx, %0 \n \t"
          "mov %%eax, %1 \n \t"
          : "=r"(high), "=r"(low)
          :: "%rax", "%rbx", "%rcx", "%rdx"
     );
     return ((uint64_t)high << 32) | low ;
}


static inline uint64_t end_timer(){
     __asm__ volatile( 
          "rdtscp \n\t"
          "mov %%edx, %0 \n \t"
          "mov %%eax, %1 \n \t"
          "cpuid \n\t"
          : "=r"(high), "=r"(low)
          :: "%rax", "%rbx", "%rcx", "%rdx"
     );
     //raw_local_irq_restore(flags);
     //preempt_enable();
     return ((uint64_t)high << 32) | low ;
}




int main(void){

     // estimate the size of the memory
     const size_t MEM_SIZE = 8LL * 1024LL * 1024LL * 1024LL;

     size_t pagesize = getpagesize();
     int skip_scale = 1023;

     // cout << numeric_limits<size_t>::max()<< endl;
     // cout << MEM_SIZE << endl;
     // cout << pagesize << endl;
     
	    
	uint8_t * test ;
	size_t test_size = MEM_SIZE/sizeof(uint8_t) ;
	
	// std::cout << "here is ok " << endl;
	
	if ( !(test =(uint8_t*) malloc(test_size)) ){
	     cerr << "Malloc test fail" << endl;
	     exit(1);
	} 
	else {
	     cout << " Mem test alloc finish :" ;
	}
	
	
	// initailize the test array
	//for (size_t i = 0; i < test_size; ++i) test[i] = 62;
	memset(test, '>', test_size); // using '>' value 62 to fillup the memory
	cout << " test init finish : " ;
	
	// create a fill up array to push test array to back store
     uint8_t * fillups;
	if ( !(fillups=(uint8_t*) malloc(test_size))) {
	     cerr << "Malloc fill fail" << endl;
	     exit(1);
	}
	else {
	     cout << " Mem fillup alloc finish :";
	}
	// for (size_t i = 0; i < test_size; ++i) fillups[i] = 1 ;
     memset(fillups, '!', test_size); // using '!' value 33 to fillup
     cout << " fillup init finish " ;

     uint8_t tmp;
     size_t istart = 0;
	for (int i = 0; i < N; ++i)
	{
		int j = rand() % pagesize;
		
		startt = start_timer();
		tmp = test[istart + j] ;
		endt = end_timer();
		if (tmp != 62) {
		     cerr << " fetched data incorrect! " << endl;
		}
		durations[i] = endt - startt;
		istart += pagesize*skip_scale / sizeof(uint8_t) ; 
		if (istart > test_size) istart -= test_size;
	}


     cout << endl;
	for (size_t i = 0; i < N; ++i)
	{
          cout << durations[i] << endl;;
	}

	free(test);
	free(fillups);
	

     return 0;
}
