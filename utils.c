#include "utils.h"


int countLetters(char *buf, FILE *f1, FILE *f2) {
    int chrCounter = 0;
    char c;

    if (f1 == NULL || f2 == NULL) {
        printf("something went wrong with the files, aborting.\n");
        return 1;
    }

    while (fgets(buf, sizeof(buf), f1) != NULL) {
        int i = 0;
        while ((c = buf[i]) != '\0') {
            if (c == '\n' || (c == EOF && chrCounter != 0)) {
                fprintf(f2, "%d\n", chrCounter);
                chrCounter = 0;
            } else {
                chrCounter++;
            }
            i++;
        }
    }
}

int copyFiles(char *buf, FILE *f1, FILE *f2){
    int i;
    char c;
    if(f1 == NULL || f2 == NULL){
        printf("something went wrong with the files, aborting. \n");
        return 1;
    }
    while(fgets(buf, sizeof(buf), f1) != NULL) {
        i = 0;
        while((c = buf[i]) != '\0'){
            putc(buf[i], f2);
            i++;
        }
    }
    return 0;
}


int pointToNextLine(FILE **p, char *buf){
    int i;
    char c;
    if(p != NULL){
        printf("something went wrong with the files, aborting. \n");
        return 1;
    }

    while(fgets(buf, sizeof(buf) - 1, p) != NULL){
        i = 0;
        while((c = buf[i]) != '\0'){
            p++;
            if(c == '\n'){
               return 0; 
            }
            i++;
        }
    }
    
}




