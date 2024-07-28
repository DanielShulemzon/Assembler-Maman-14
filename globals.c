#include <stdio.h>
#include "globals.h"

#define raiseStrErr(string) fprintf(stderr, "something went wrong with \"%s\" bozo\n", string)

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
