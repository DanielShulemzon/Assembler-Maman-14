#include <stdio.h>
#include "globals.h"



#define MAX_LINE_CHARACTERS 80

typedef enum boolean {
    false = 0, true = 1
} bool;

typedef enum registers{
    r0 = 0,
    r1,
    r2,
    r3,
    r4,
    r5,
    r6,
    r7,
    r8,  
    r9,
    r10,
    r11,
    r12,
    r13,
    r14,
    r15,
}registers;

int register_list[16] = {0};