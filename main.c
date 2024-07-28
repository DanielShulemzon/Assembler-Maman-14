#include <stdio.h>
#include "utils.h"

// run command:./main f1.txt f2.txt

int main(int argc, char *argv[]) {
    int i = 0;
    FILE *f1, *f2;
    char buf[50] = {0};
    if(argc != 3){
        printf("i said 2.\n");
        printf("you entered %d you aumbass.\n", argc - 1);
        return 1;
    }
    f1 = fopen(argv[1], "r");
    f2 = fopen(argv[2], "w");
    
    if (f1 == NULL || f2 == NULL) {
        printf("failed");
        return 1;
    }
    if(countLetters(buf, f1, f2) == 1) return 1;


    fclose(f1);
    fclose(f2);

    return 0;
}