#include <stdio.h>
#include <iostream>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <cerrno>

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
 1. use O_direct to change the read mode, so read does not cache the file
    into file cache but to the user space memory.
 2. read the file once for loading file to file cache.
 3. read the fime sequential and get the average time.
*/ 
#define BLOCKSIZE 64 

int main(int argc, char **argv){

    string filename = "";
    filename += argv[1];
    uint64_t sum = 0;
    uint64_t start, end;
    int file;
    // open the file 
    file = open(filename.c_str(), O_SYNC | O_DIRECT);
    // check for error
    if(file < 0){
        printf("Error opening file: %s\n", strerror(errno));
    }
    cout << "open file " << filename << endl;
    char* buf = (char *) malloc(BLOCKSIZE);
    int filesize = lseek(file, 0, SEEK_END);
    int blocknum = filesize / BLOCKSIZE;
    lseek(file, 0, SEEK_SET);
    // read the file again
    for(int i = 0; i < blocknum; i++){
        start = start_timer();
        read(file, buf, BLOCKSIZE);
        end = end_timer();
        sum += (end - start);
    }
    cout << "average reading time " << sum / blocknum << endl;
    close(file);
    free(buf);
    return 0;
}
