#ifndef _GLOBALS_H
#define _GLOBALS_H

#define HEAP_CAPACITY 64
#define MAX_LINE_LENGTH 80
#define OP_NUM 16

typedef enum booleans {
	false = 0, true = 1
} bool;

#define raiseStrErr(string) fprintf(stderr, "something went wrong with \"%s\" bozo\n", string)


typedef enum registers{
    r0 = 0,
    r1,
    r2,
    r3,
    r4,
    r5,
    r6,
    r7,
    NONE_REG = -1
}registers;


typedef enum operations {
	MOV_OP = 0,
	CMP_OP = 1,

	ADD_OP = 2,
	SUB_OP = 2,

	LEA_OP = 4,
    //first operand group
	CLR_OP = 5,
	NOT_OP = 5,
	INC_OP = 5,
	DEC_OP = 5,

	JMP_OP = 9,
	BNE_OP = 9,
	JSR_OP = 9,
    //second operand group
	RED_OP = 12,
	PRN_OP = 13,

	RTS_OP = 14,
	STOP_OP = 15,

	/** Failed/Error */
	NONE_OP = -1
} operations;

typedef struct line_info{
	long line_number;
	char *file_name;
	char *content;
} line_info;

#endif