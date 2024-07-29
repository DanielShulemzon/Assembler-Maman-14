#include <stdio.h>
#include "utils.h"
#include "globals.h"

// run command:./main f1.txt f2.txt

int main(int argc, char *argv[]) {
    int i;
    int c;
    bool succeeded = true;

    for(i = 1; i < argc && succeeded; ++i){

        succeeded = process_file(argv[i]);
    }
    return 0;
}

bool process_file(char *filename){
    bool valid_file = true;
    int i;
    char c;
    

    // initiating first check.

    
}