/* file that contains global variables and values.*/
#ifndef _GLOBALS_H
#define _GLOBALS_H

#define MAX_LINE_LENGTH 80

#define IC_INIT_VALUE 100

#define CODE_ARR_IMG_LENGTH 1200

#define TABLE_INITIAL_CAPACITY 8


typedef enum booleans {
	false = 0, true = 1
} bool;


typedef enum addressing_types {
	/* Immediate addressing (0) */
	IMMEDIATE_ADDR = 0,
	/* Direct addressing (1) */
	DIRECT_ADDR = 1,
	/* Relative addressing (2) */
	INDIRECT_REGISTER_ADDR = 2,
	/* Register addressing */
	DIRECT_REGISTER_ADDR = 3,
	/* Failed/Not detected addressing */
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
}reg;


typedef enum {
    /* first operand group */
	MOV_OP = 0,
	CMP_OP = 1,

	ADD_OP = 2,
	SUB_OP = 3,

	LEA_OP = 4,
    /* second operand group */
	CLR_OP = 5,
	NOT_OP = 6,
	INC_OP = 7,
	DEC_OP = 8,

	JMP_OP = 9,
	BNE_OP = 10,
	RED_OP = 11,
	PRN_OP = 12,
	JSR_OP = 13,
    /* third operand group */
	RTS_OP = 14,
	STOP_OP = 15,

	/* Failed/Error */
	NONE_OP = -1
} opcode;



typedef struct code_word {
	/* ARE field.*/
	unsigned int ARE: 3;
	/* destination and source addressing types.*/
	unsigned int dest_addressing: 4;
	unsigned int src_addressing: 4;
	/* opcode number. */
	unsigned int opcode: 4;

} code_word;


typedef struct data_word {
	/* ARE field. */
	unsigned int ARE: 3;
	/* The data content itself. */
	unsigned long data;
} data_word;

typedef struct register_word {
	/* ARE field. */
	unsigned int ARE: 3;
	/* destination and source registers (if exist)*/
	unsigned int dest_register: 3;
	unsigned int src_register: 3;

} register_word;

/* union to track which type of word is in the machine_word. */
typedef enum {
    CODE_UNION_TYPE,
    DATA_UNION_TYPE,
    REG_UNION_TYPE
} union_type;

typedef struct machine_word {
	/* if it represents code (not additional data), this field contains the total length required by the code. if it's either data or reg, this field is 0. */
	short length;
	/* The content can be code/data/reg */
	union word {
		register_word *reg;
		data_word *data;
		code_word *code;
	} word;

	union_type type; /* type of the union. */
} machine_word;

typedef enum instruction {
	/* .data instruction */
	DATA_INST,
	/* .extern instruction */
	EXTERN_INST,
	/* .entry instruction */
	ENTRY_INST,
	/* .string instruction */
	STRING_INST,
	/* Not found */
	NONE_INST,
	/* Parsing/syntax error */
	ERROR_INST
} instruction;

/* structure that contains info about a line.*/
typedef struct line_info{
	/* number of current line. */
	unsigned int line_number;
	/* the name of the file. */
	char *file_name;
	/* the line itself. */
	char *content;
} line_info;

#endif