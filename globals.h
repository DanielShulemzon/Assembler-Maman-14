#ifndef _GLOBALS_H
#define _GLOBALS_H

#define MAX_LINE_LENGTH 80
#define OP_NUM 16
#define BUFFER_SIZE 256
#define IC_INIT_VALUE 100


typedef enum booleans {
	false = 0, true = 1
} bool;


typedef enum addressing_types {
	/** Immediate addressing (0) */
	IMMEDIATE_ADDR = 0,
	/** Direct addressing (1) */
	DIRECT_ADDR = 1,
	/** Relative addressing (2) */
	RELATIVE_ADDR = 2,
	/** Register addressing */
	REGISTER_ADDR = 3,
	/** Failed/Not detected addressing */
	NONE_ADDR = -1
} addressing_type;

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


typedef struct code_word {
	/* First byte: ARE+funct */
	unsigned int ARE: 3;
	unsigned int funct: 5;
	/* 2nd byte: destination+addressing, source */
	unsigned int dest_register: 3;
	unsigned int dest_addressing: 2;
	unsigned int src_register: 3;
	/* Third byte: source addressing, opcode */
	unsigned int src_addressing: 2;
	unsigned int opcode: 6;

} code_word;


typedef struct data_word {
	unsigned int ARE: 3;
	/* The data content itself (a method for putting data into these field is defined) */
	unsigned long data;
} data_word;

typedef struct machine_word {
	/* if it represents code (not additional data), this field contains the total length required by the code. if it's data, this field is 0. */
	short length;
	/* The content can be code or data */
	union word {
		data_word *data;
		code_word *code;
	} word;
} machine_word;

typedef enum instruction {
	/** .data instruction */
	DATA_INST,
	/** .extern instruction */
	EXTERN_INST,
	/** .entry instruction */
	ENTRY_INST,
	/** .string instruction */
	STRING_INST,
	/** Not found */
	NONE_INST,
	/** Parsing/syntax error */
	ERROR_INST
} instruction;

typedef struct line_info{
	unsigned int line_number;
	char *file_name;
	char *content;
} line_info;

#endif