#include <stdio.h>
#include <iostream>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

using namespace std;
unsigned int high, low, id;

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


/**
 the idea of code is
 1. receive two arguments: file and t size
 2. open the file and read only first t bytes of the file
 3. close the file and reopen it
 4. read first t bytes of file again
 5. calculate the average read time
*/ 

int main(int argc, char **argv){

    uint64_t test = (uint64_t)stoi(argv[2]);
    uint64_t measuresize = test*1024*1024;
    uint64_t BLOCKSIZE = 128 * 1024;

    string filename = "";
    filename += argv[1];
    uint64_t sum = 0;
    FILE * file;
    // open the file 
    file = fopen(filename.c_str(), "r");
    char * inbuf = (char*) malloc(BLOCKSIZE);
    uint64_t start, end;
    uint64_t step = measuresize / BLOCKSIZE;

    // put data of file into ram by reading through it 
    if(file != NULL){
        for(int i = 0; i < step; i++){
            fread(inbuf, BLOCKSIZE, 1, file);
        }
    }
    // close the file and reopen it
    fclose(file);
    file = fopen(filename.c_str(), "r");
    // read through the file again
    if(file != NULL){
        for(int i = 0; i < step; i++){
            start = start_timer();
            fread(inbuf, BLOCKSIZE, 1, file);
            end = end_timer();
            sum += (end - start);
        }
        // get the average time
        cout << "reading time: " << sum / step << endl;
    
    }
    // close file and free memory
    fclose(file);
    free(inbuf);
    return 0;
}
