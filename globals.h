#ifndef _GLOBALS_H
#define _GLOBALS_H

#define MAX_LINE_LENGTH 80
#define OP_NUM 16
#define BUFFER_SIZE 256


typedef enum booleans {
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
    NONE_REG = -1
}registers;


typedef enum {
	MOV_OP = 0,
	CMP_OP = 1,

	ADD_OP = 2,
	SUB_OP = 3,

	LEA_OP = 4,
    //first operand group
	CLR_OP = 5,
	NOT_OP = 6,
	INC_OP = 7,
	DEC_OP = 8,

	JMP_OP = 9,
	BNE_OP = 10,
	JSR_OP = 11,
    //second operand group
	RED_OP = 12,
	PRN_OP = 13,

	RTS_OP = 14,
	STOP_OP = 15,

	/** Failed/Error */
	NONE_OP = -1
} op_enum;

typedef struct operation{
	char *name;
	op_enum op_number;

}operation;



typedef struct line_info{
	unsigned int line_number;
	char *file_name;
	char *content;
} line_info;

#endif