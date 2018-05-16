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
#include <sys/mman.h>


using namespace std;


// global variable, record the start and end CPU time stampe
static uint64_t startt, endt, duration;
unsigned int high, low, id;
//unsigned long flags;
//struct sched_param params;
const size_t N = 1000;
uint64_t durations[N];


pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER ; 
pthread_cond_t cond = PTHREAD_COND_INITIALIZER ; 

int cond_x;




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





// a functon doing nothing 
void nothing(){
     endt = end_timer();
 }
void nothing(int a1){
     endt = end_timer();
     a1 ++ ;
 }
void nothing(int a1, int a2){
     endt = end_timer();
     a1 ++ ;
     a2 ++ ;
 }
void nothing(int a1, int a2, int a3){
     endt = end_timer();
     a1 ++ ;
     a2 ++ ;
     a3 ++ ;
 }
void nothing(int a1, int a2, int a3, int a4){
     endt = end_timer();
     a1 ++ ;
     a2 ++ ;
     a3 ++ ;
     a4 ++ ;
 }
void nothing(int a1, int a2, int a3, int a4, int a5){
     endt = end_timer();
     a1 ++ ;
     a2 ++ ;
     a3 ++ ;
     a4 ++ ;
     a5 ++ ;
 }
void nothing(int a1, int a2, int a3, int a4, int a5, int a6){
     endt = end_timer();
     a1 ++ ;
     a2 ++ ;
     a3 ++ ;
     a4 ++ ;
     a5 ++ ;
     a6 ++ ;
 }
void nothing(int a1, int a2, int a3, int a4, int a5, int a6, int a7){
     endt = end_timer();
     a1 ++ ;
     a2 ++ ;
     a3 ++ ;
     a4 ++ ;
     a5 ++ ;
     a6 ++ ;
     a7 ++ ;
 }



// a function for print the time in child thread
void * cal_time_in_child(void * some ){
     endt = cpu_cycle_RDTSC();
     duration = endt - startt;
     //cout << " duration is " << duration << endl;
     // pthread_exit(NULL);
}



// Read measurement
void read_measure(){
       
     
     for(int i=0; i < N; i++){

     //      // startt = cpu_cycle_RDTSC();
     //      // endt = cpu_cycle_RDTSC();          
          startt = start_timer();
          endt = end_timer();
          durations[i] = endt - startt;
     };
     //cout << " Reading Measurement : " << endl;
     for(int i=0; i< N; i++){
          cout << durations[i] << endl;
     }

     // doing nothing will gives the read overhead
     //startt = start_timer();
     //endt = end_timer();

     //duration = endt - startt;
     //cout << duration << endl;
};



// Procedure call measurement; looping 1k times
// void procedure_measure(const char* count ){
//      int a1=0, a2=0, a3=0, a4=0, a5=0, a6=0, a7=0;

//      uint64_t duration[1000];  

//      if (*count == '0'){
//           for(int i=0; i < 1000; i++){
//                // startt = cpu_cycle_RDTSC();
//                // endt = cpu_cycle_RDTSC();          
//                startt = start_timer();
//                nothing();
//                endt = end_timer();
//                duration[i] = endt - startt;
//           };
//           cout << " Procedure call 0 argument measurement : " << endl;
//      } else if (*count == '1'){
//           for(int i=0; i < 1000; i++){
//                // startt = cpu_cycle_RDTSC();
//                // endt = cpu_cycle_RDTSC();          
//                startt = start_timer();
//                nothing(a1);
//                endt = end_timer();
//                duration[i] = endt - startt;
//           };
//           cout << " Procedure call 1 argument measurement : " << endl;
//      } else if (*count == '2'){
//           for(int i=0; i < 1000; i++){
//                // startt = cpu_cycle_RDTSC();
//                // endt = cpu_cycle_RDTSC();          
//                startt = start_timer();
//                nothing(a1, a2);
//                endt = end_timer();
//                duration[i] = endt - startt;
//           };
//           cout << " Procedure call 2 argument measurement : " << endl;  
//      } else if (*count == '3'){
//           for(int i=0; i < 1000; i++){
//                // startt = cpu_cycle_RDTSC();
//                // endt = cpu_cycle_RDTSC();          
//                startt = start_timer();
//                nothing(a1, a2, a3);
//                endt = end_timer();
//                duration[i] = endt - startt;
//           };
//           cout << " Procedure call 3 argument measurement : " << endl;
//      } else if (*count == '4'){
//           for(int i=0; i < 1000; i++){
//                // startt = cpu_cycle_RDTSC();
//                // endt = cpu_cycle_RDTSC();          
//                startt = start_timer();
//                nothing(a1, a2, a3, a4);
//                endt = end_timer();
//                duration[i] = endt - startt;
//           };
//           cout << " Procedure call 4 argument measurement : " << endl;
//      } else if (*count == '5'){
//           for(int i=0; i < 1000; i++){
//                // startt = cpu_cycle_RDTSC();
//                // endt = cpu_cycle_RDTSC();          
//                startt = start_timer();
//                nothing(a1, a2, a3, a4, a5);
//                endt = end_timer();
//                duration[i] = endt - startt;
//           };
//           cout << " Procedure call 5 argument measurement : " << endl;
//      } else if (*count == '6'){
//           for(int i=0; i < 1000; i++){
//                // startt = cpu_cycle_RDTSC();
//                // endt = cpu_cycle_RDTSC();          
//                startt = start_timer();
//                nothing(a1, a2, a3, a4, a5, a6);
//                endt = end_timer();
//                duration[i] = endt - startt;
//           };
//           cout << " Procedure call 6 argument measurement : " << endl;
//      } else if (*count == '7'){
//           for(int i=0; i < 1000; i++){
//                // startt = cpu_cycle_RDTSC();
//                // endt = cpu_cycle_RDTSC();          
//                startt = start_timer();
//                nothing(a1, a2, a3, a4, a5, a6, a7);
//                endt = end_timer();
//                duration[i] = endt - startt;
//           };
//           cout << " Procedure call 7 argument measurement : " << endl;
//      } 
//      for(int i=0; i< 1000; i++){
//           cout << duration[i] << endl;
//      }     
// }

// Procedure call measurement, no loops
void procedure_measure(const char* count ){
     int a1=0, a2=0, a3=0, a4=0, a5=0, a6=0, a7=0;

     if (*count == '0'){         
          for(int i=0; i < N; i++){   
               startt = start_timer();
               nothing();             
               //endt = end_timer();
               durations[i] = endt - startt;
          };
          // cout << " Procedure call 0 argument measurement : " << endl;
     } else if (*count == '1'){    
          for(int i=0; i < N; i++){   
               startt = start_timer();
               nothing(a1);             
               //endt = end_timer();
               durations[i] = endt - startt;
          };
     } else if (*count == '2'){  
          for(int i=0; i < N; i++){   
               startt = start_timer();
               nothing(a1, a2);             
               //endt = end_timer();
               durations[i] = endt - startt;
          };
     } else if (*count == '3'){          
          for(int i=0; i < N; i++){   
               startt = start_timer();
               nothing(a1, a2, a3);             
               //endt = end_timer();
               durations[i] = endt - startt;
          };
     } else if (*count == '4'){         
          for(int i=0; i < N; i++){   
               startt = start_timer();
               nothing(a1, a2, a3, a4);             
               //endt = end_timer();
               durations[i] = endt - startt;
          };
     } else if (*count == '5'){      
          for(int i=0; i < N; i++){   
               startt = start_timer();
               nothing(a1, a2, a3, a4, a5);            
               //endt = end_timer();
               durations[i] = endt - startt;
          };
     } else if (*count == '6'){   
          for(int i=0; i < N; i++){   
               startt = start_timer();
               nothing(a1, a2, a3, a4, a5, a6);             
               //endt = end_timer();
               durations[i] = endt - startt;
          };
     } else if (*count == '7'){      
          for(int i=0; i < N; i++){   
               startt = start_timer();
               nothing(a1, a2, a3, a4, a5, a6, a7);             
               //endt = end_timer();
               durations[i] = endt - startt;
          };
     } 
     //cout << duration << endl;
     for(int i=0; i< N; i++){
          cout << durations[i] << endl;
     }     

}



void system_call_measure(){
     for(int i=0; i < N; i++){
          startt = start_timer();
     //      // the minimal system call is "syscall" to interrupt/trap OS
     //      __asm__ volatile ("syscall");
          getpid();
          endt = end_timer();
          durations[i] = endt - startt;
     };
     for(int i=0; i< N; i++){
          cout << durations[i] << endl;
     }   

     //startt = start_timer();
     // the minimal system call is "syscall" to interrupt/trap OS
     //__asm__ volatile ("syscall");
     //getpid();
     //endt = end_timer();
     //duration = endt - startt;
     //cout << duration << endl;        
}


void process_creation_measure(){

     pid_t pid, w;
     int status;
     for(int i=0; i < N; i++){
          // startt = start_timer();
          // movl and syscall means fork and kill, which leaves the time to create a process
          //__asm__ volatile("movl $2, %eax\n\t"  
          //                   "syscall \n\t");
          startt = start_timer();
          // pid = vfork();   // VFORK suspending the parent process, FORK does not.
          pid = fork();
          //cout << pid << endl;
          if (pid == 0){
               // endt = end_timer();
               // a context switch + process creation is forced when the child process is run (parent suspended)
               //cout << start_timer() << endl;
               //wait(NULL);
               // endt = end_timer();
               _exit(0);
          } else{                            
               // w = waitpid(pid, &status, WUNTRACED | WCONTINUED);
               // wait(NULL);
               endt = end_timer();
               durations[i] = endt - startt ;
               wait(NULL);
          }
          
     };
     
     // this is process creation + context switch !!!
     for(int i=0; i< N; i++){
          cout << durations[i] << endl;
     }  
     
     //startt = start_timer();
     //endt = end_timer();
     //duration = endt - startt ;
     //cout << duration << endl;
     exit(EXIT_SUCCESS);
}


static void * pthread_trap(void* some){
     //__asm__ volatile("syscall \n\t");
     // return NULL;
     endt = end_timer();
     pthread_exit(NULL);
}

// a function for print the time in child thread
static int clone_trap(void * some ){
     //endt = end_timer();
     //duration = endt - startt;
     //cout << " duration is " << duration << endl;
     // pthread_exit(NULL);
     _exit(2);
}

void pthread_creation_measure(){ 
     pthread_t threadid ;
     
     void* child_stack, *child_stack_top;
     size_t STACK_SIZE = 256;
     int thread_pid;
     
     for(int i=0; i < N; i++){

          start_timer();
          end_timer();

          uint64_t special_start = start_timer();
          //child_stack = malloc(STACK_SIZE);
          //child_stack_top = child_stack+STACK_SIZE ;
          // thread create by pthread_create
          // need further tuning here          
          pthread_create(&threadid , NULL, &pthread_trap, NULL);
          
          //thread_pid = clone(&clone_trap, child_stack_top, CLONE_VFORK, NULL);
          
     
          startt = end_timer();
          
          pthread_join(threadid, NULL);
          
          if (startt < endt) {
          // if (true) {
               durations[i] = startt - special_start;
          } else{
               durations[i] = endt - special_start;
          }
     };     
     for(int i=0; i< N; i++){
          cout << durations[i] << endl;
     }      
     // pthread_exit(NULL);
}


void process_ctxswitch_measure(){
     pid_t pid, ppid=getpid() ;
     uint64_t * startt_ptr = (uint64_t*) mmap(NULL, sizeof(uint64_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1 ,0 );

     *startt_ptr = 0;

     for(int i=0; i < N; i++){
          // startt = start_timer();
          // vfork creates a process and hault the parent process.
          pid = fork();   // SHOULD BE VFORK!!!!     
          if (pid == 0){
               * startt_ptr = start_timer();
               // cout << startt_ptr << " c : " << *startt_ptr << endl;

               // kill(ppid, SIGUSR2);
               // sigsuspend(&sigset);
               // a context switch is forced when the child process is terminated
               // cout << " c i = " << i << endl;
               _exit(0);
               // return;
          } else{
               // startt = start_timer();

               wait(NULL);
               // cout << " p i = " << i << endl;
               // cout << startt_ptr << " p : " << *startt_ptr << " " << startt ;
               
               endt = end_timer();
               // cout << " " << endt << endl;
               // kill(pid, SIGUSR1);
               // wait(NULL);  // wait until the child process returns
               // w = waitpid(pid, &status, WUNTRACED | WCONTINUED);               
               durations[i] = endt - *startt_ptr  ;
          }
          
     }
     for(int i = 0; i<N; i++){
          cout << durations[i] << endl;
     }
     return ;
}







void* t1_fn(void* some){

for(int i =0; i < N; i++){

     // cout << " t1 1" << endl;
     pthread_mutex_lock(&mut); // lock thread
     // cout << " t1: before signal 1 " << endl;
     while (cond_x == 0 ) {
          cond_x = 1;
          pthread_cond_signal(&cond); // signal 1 - let t2 goes synchronized
          pthread_cond_wait(&cond, &mut); // wait for the t2 until it signal 2
     }

     // receive signal 2
     startt = start_timer();
     pthread_cond_signal(&cond);  // sendt t2 that timer has startt - signal 3
     pthread_cond_wait(&cond, &mut); // wait for t2 until signal 4


     endt = end_timer();      
     durations[i] = (uint64_t) ((endt - startt)/2);
     // duration = endt - startt;
     //cout << (duration/2) << endl; // two context switches
     pthread_mutex_unlock(&mut);
     // cout << " fn 1 : iter " << i << endl;
};

cond_x = 3;

for(int i =0; i < N; i++){
     cout << durations[i] << endl;
};

     pthread_exit(NULL);
};




void* t2_fn(void* some){

// for(int i =0; i < N; i++){
while(cond_x !=3){
     // cout << " t2 1" << endl;
     pthread_mutex_lock(&mut);
     while (cond_x != 1 ){
          pthread_cond_wait(&cond, &mut); // wait t1 for signal 1
     }


     cond_x == 2 ;
     pthread_cond_signal(&cond); // know t1 is ready. t1 start timer please - signal 2 
     pthread_cond_wait(&cond, &mut); // wait t1 signal 3


     //cond_x++;
     cond_x == 0 ;
     pthread_cond_signal(&cond); // inform t1 to end timer - signal 4 //
     pthread_mutex_unlock(&mut);          
     

     // cout << " fn 2 : iter " << i << endl;
};
     
     pthread_exit(NULL);
};





void pthread_ctxswitch_measure(){
     cond_x = 0;
     pthread_t t1, t2; 

     int rc;
     //cout << " create thread 1" << endl;
     pthread_create(&t1 , NULL, &t1_fn, NULL);
     //  if (rc) {
     //     cout << "Error:unable to create thread," << rc << endl;
     //     exit(-1);
     //  }     

     //cout << " create thread 2" << endl;
     pthread_create(&t2 , NULL, &t2_fn, NULL);   
     //  if (rc) {
     //     cout << "Error:unable to create thread," << rc << endl;
     //     exit(-1);
     //  }     
     pthread_join(t1, NULL);
     pthread_join(t2, NULL);
     //pthread_exit(NULL);  
}




int main(int argc, char *argv[]){


     // set process/thread priority should be launched by super-user

     //params.sched_priority = sched_get_priority_max(SCHED_FIFO);
     //sched_getparam(0, &params);
     //params.sched_priority = sched_get_priority_max(SCHED_FIFO)    ;
     //sched_setparam(0, &params);
     
     // cout << getpid() << " " << SCHED_FIFO << " " << params.sched_priority ;
     
     //int ret = sched_setscheduler(0, SCHED_FIFO, &params);

     //if (ret != 0) {
     //     cout << " Scheduling unsuccessful with restult = " << ret << endl;
     //};

     //pthread_t threadid ;
     
   
     //uint64_t start,end;
     if (argc > 1){
          if ( *argv[1] == '1') {
               read_measure();
          } else if ( *argv[1] == '2' ) {
               if ( argc ==3) {
                    procedure_measure(argv[2]);
               } else {
                    cout << " need more paramters" << endl;
               }
          } else if ( *argv[1] == '3' ) {
               system_call_measure();
          } else if ( *argv[1] == '4' ) {
               process_creation_measure();
          } else if ( *argv[1] == '5' ) {
               pthread_creation_measure();
          } else if ( *argv[1] == '6' ) {
               process_ctxswitch_measure();
          } else if ( *argv[1] == '7' ) {
               pthread_ctxswitch_measure();
          } 


     }
     return 0;
}
