#include <stdint.h>
#include <iostream>
#include <string.h>

using namespace std;
#define BLOCKSIZE 1024*1024
int main(int argc, char **argv){
    if(argc != 2){
        cout << "need a argument for size of file like 'mfile 1'" << endl;
        return 0;
    }
    string filename = "testfile";
    filename += argv[1];
    filename += "G.txt"; 
    FILE * file = fopen(filename.c_str(), "w");
    char * buffer = (char*) malloc(BLOCKSIZE);
    uint64_t numBlock = 1024 * stoi(argv[1]);
    if(file != NULL){
        for(int j = 0; j < BLOCKSIZE; j++){
            buffer[j] = 'h';
        }
        for(int j = 0; j < numBlock; j++){
            fwrite(buffer, BLOCKSIZE, 1, file);
        }
    }
    return 0;
}
