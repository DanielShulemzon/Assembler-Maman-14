#ifndef _GLOBALS_H
#define _GLOBALS_H

typedef enum booleans {
	false = 0, true = 1
} bool;

#define raiseStrErr(string) fprintf(stderr, "something went wrong with \"%s\" bozo\n", string)

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
    NONE_REG = -1
}registers;

int register_list[16] = {0};

#endif