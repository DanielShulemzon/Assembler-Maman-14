#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "first_pass.h"
#include "utils.h"
#include "instructions.h"
#include "code.h"

/*
 *  Handles a line of code, updating the instruction counter and code image.
 *  @param line - Information about the current line.
 *  @param ic - the current ic.
 *  @param code_img - Pointer to the code image array.
 *  @param label_found - Indicates if a label was found on the line.
 *  @return - True if the line was handled successfully, false otherwise.
 */
static bool handle_code_line(line_info line, long *ic, machine_word **code_img, bool label_found);

/*
 *  Builds extra codewords for operands.
 *  @param line - Information about the current line.
 *  @param code_img - Pointer to the code image array.
 *  @param ic - the current ic.
 *  @param operands - 2d array which contains the operands in the line.
 *  @param op_count - Number of operands.
 */
static void fpass_build_extra_codewords(line_info line, machine_word **code_img, long *ic, char operands[2][MAX_LINE_LENGTH], int op_count);

/*
 *  Builds a code word from a destination operand.
 *  @param line - Information about the current line.
 *  @param code_img - Pointer to the code image array.
 *  @param ic - the current ic.
 *  @param operand - The destination operand string.
 *  @param addr - The addressing type of the operand.
 */
static void fpass_build_word_from_dest_operand(line_info line, machine_word **code_img, long *ic, char *operand, addressing_type addr);

/*
 *  Builds a code word from a source operand.
 *  @param line - Information about the current line.
 *  @param code_img - Pointer to the code image array.
 *  @param ic - the current ic.
 *  @param operand - The source operand string.
 *  @param addr - The addressing type of the operand.
 */
static void fpass_build_word_from_src_operand(line_info line, machine_word **code_img, long *ic, char *operand, addressing_type addr);

/*
 *  Extracts register from operand based on it's addressing type.
 *  @param operand - The operand string.
 *  @param addressing - The addressing type of the operand.
 *  @return - The register corresponding to the operand and addressing type.
 */
static reg get_register_by_addr(const char *operand, addressing_type addressing);



bool fpass_process_line(line_info line, machine_word **code_img, long *data_img, long *ic, long *dc, table *symbol_table){
    bool label_found = false;
    int length;
    char symbol[MAX_LINE_LENGTH] = {0}, tokenized_line[MAX_LINE_LENGTH + 2], *first_word, *token;
    instruction inst;

    strcpy(tokenized_line, line.content);

    first_word = strtok(tokenized_line, " \t\n"); /* discluding label. */

    if(first_word == NULL || first_word[0] == EOF || first_word[0] == ';'){
        /* the line is empty or a comment */
        return true;
    }

    /* check if label */
    length = strlen(first_word);
    if(length > 0 && first_word[length-1] == ':'){
        /* possible label found! */

        first_word[length - 1] = '\0'; /* remove the ':' from the label */

        strcpy(symbol, first_word);

        if (!is_valid_label_name(symbol)) {
            printf_line_error(line, "label name \"%s\" is illegal.", symbol);
            return false;
        }
        first_word = strtok(NULL, " \t\n"); /* make first_word the first word after the label. */
        
        if(find_by_types(symbol_table, symbol, 3, CODE_SYMBOL, DATA_SYMBOL, EXTERNAL_SYMBOL)){
            printf_line_error(line, "label name already exists.");
            return false;
        } 

        label_found = true;
    }


    inst = get_instruction_from_word(first_word); /* get the instruction type of the line (NONE_INST if it's code.) */
    if(inst == ERROR_INST){ /* if word starts with a dot but is not a legal instruction. */
        printf_line_error(line, "instruction %s is illegal.", first_word);
        return false;
    }

    if(inst != NONE_INST){ /* if instruction*/
        if(label_found && (inst == DATA_INST || inst == STRING_INST)){
            add_table_item(symbol_table, symbol, *dc, DATA_SYMBOL);
        }
        if(inst == DATA_INST){
            return handle_data_instruction(line, dc, data_img);
        }
        if(inst == STRING_INST){
            return handle_string_instruction(line, dc, data_img);
        }
        if(inst == EXTERN_INST){
            token = strtok(NULL, " \t\n");
            strcpy(symbol, token); /* get label name provided */

            if(!is_valid_label_name(symbol)){
                printf_line_error(line, "label name %s is illegal.", symbol);
                return false;
            }
            if(find_by_types(symbol_table, symbol, 3, CODE_SYMBOL, DATA_SYMBOL, EXTERNAL_SYMBOL)){
                printf_line_error(line, "label name already exists.");
                return false;
            }
            if(strtok(NULL, " \t\n") != NULL){
                printf_line_error(line, "more than one arguement was given to .extern instruction.");
            }

            add_table_item(symbol_table, symbol, 0, EXTERNAL_SYMBOL); /* Extern value is defaulted to 0 */
            return true;
        }
        /* entry will be handled in the second pass. */
    }
    else{ /* only option left to check is a command */
        if(label_found){
            add_table_item(symbol_table, symbol, *ic, CODE_SYMBOL);
        }
        return handle_code_line(line, ic, code_img, label_found);
    }
    return true;
}

static bool handle_code_line(line_info line, long *ic, machine_word **code_img, bool label_found){
    char op_name[8] = {0}, operands[2][MAX_LINE_LENGTH] = {0};
    opcode curr_opcode;
    code_word *codeword;
    long ic_before;
    int op_count, index = 0, i;
    machine_word *word_to_write;

    if(label_found){
        SKIP_WHITE_SPACES(line.content, index);
        while(line.content[index] != '\n' && !isspace(line.content[index])){
            index++;
        }
    } /* if found label skip it. */

    SKIP_WHITE_SPACES(line.content, index);
    i = 0;
    while(line.content[index] != '\n' && !isspace(line.content[index])){
        if(i >= 4){
            printf_line_error(line, "unrecognized command: command too long.");
            return false;
        }
        op_name[i++] = line.content[index++];
    } /* extract the opcode name */
    op_name[i] = '\0'; /* null terminate the string. */

    curr_opcode = get_opcode_by_name(op_name);
    if(curr_opcode == NONE_OP){
        printf_line_error(line, "command %s is unrecognized.", op_name);
        return false;
    }

    if(!extract_operands(line, index, &op_count, operands)){ /* fill the operands 2d array.*/
        return false;
    }

    if ((codeword = get_code_word(line, curr_opcode, op_count, operands)) == NULL) { /* get the code word of the operand.*/
		return false;
	}

    /* remember the position of the codeword. */
    ic_before = *ic;

    word_to_write = (machine_word *) malloc_with_check(sizeof(machine_word));
	(word_to_write->word).code = codeword;
    word_to_write->type = CODE_UNION_TYPE;
	code_img[(*ic) - IC_INIT_VALUE] = word_to_write;

    if(op_count){ /* If there's at least 1 operand */
		fpass_build_extra_codewords(line, code_img, ic, operands, op_count);
	}

    (*ic)++;

    code_img[ic_before - IC_INIT_VALUE]->length = (*ic) - ic_before; /* calculate L and update the length. */

	return true; /* line_valid is initiallized to true but if any operand is illegal it would be false. */
}


static void fpass_build_extra_codewords(line_info line, machine_word **code_img, long *ic, char operands[2][MAX_LINE_LENGTH], int op_count) {
	machine_word *word_to_write;
    addressing_type dest_addressing = NONE_ADDR, src_addressing = NONE_ADDR;
    reg src_reg, dest_reg;

    /*if the opcode recieves two operands, the first operand will be source operand and the second will be destination operand. 
        but if the opcode recieves one operand it will only be destination operand. */
    if(op_count == 1){
        dest_addressing = get_addressing_type(operands[0]);
    }
    else if(op_count == 2){
        src_addressing = get_addressing_type(operands[0]);
        dest_addressing = get_addressing_type(operands[1]);
    }
	
    /* handle both operands together. */
    if((dest_addressing == DIRECT_REGISTER_ADDR || dest_addressing == INDIRECT_REGISTER_ADDR) &&
                        (src_addressing == DIRECT_REGISTER_ADDR || src_addressing == INDIRECT_REGISTER_ADDR)){ /* check if both addressing types are registers. */

        /* if the operands start with a star (INDIRECT_REGISTER_ADDR) we skip it and find the operand*/                
        src_reg = get_register_by_addr(operands[0], src_addressing);
        dest_reg = get_register_by_addr(operands[1], dest_addressing);
        
        (*ic)++;

        word_to_write = (machine_word *) malloc_with_check(sizeof(machine_word));
        word_to_write->length = 0; /* not a codeword. */
        (word_to_write->word).reg = build_register_word(src_reg, dest_reg);
        word_to_write->type = REG_UNION_TYPE;

        code_img[(*ic) - IC_INIT_VALUE] = word_to_write;
        return;
    }

    /* handle each operand seperately. starting with first operand. */

    (*ic)++;

    /* first operand type is determined by the number of operands. */
    if(op_count == 2)
        fpass_build_word_from_src_operand(line, code_img, ic, operands[0], src_addressing);
    else
        fpass_build_word_from_dest_operand(line, code_img, ic, operands[0], dest_addressing);
        
    
    /* first operand done, now check if there's a second operand and process it. */

    if(op_count == 1) return;

    (*ic)++;
    
    fpass_build_word_from_dest_operand(line, code_img, ic, operands[1], dest_addressing);

    return;
}

static void fpass_build_word_from_dest_operand(line_info line, machine_word **code_img, long *ic, char *operand, addressing_type addr){
    machine_word *word_to_write;
    char *ptr;
    reg dest_reg;

    if(addr == DIRECT_REGISTER_ADDR || addr == INDIRECT_REGISTER_ADDR){
        /* if the operands start with a star (INDIRECT_REGISTER_ADDR) we skip it and find the operand*/                
        dest_reg = get_register_by_addr(operand, addr);

        word_to_write = (machine_word *) malloc_with_check(sizeof(machine_word));
        word_to_write->length = 0; /* not a codeword. */
        (word_to_write->word).reg = build_register_word(NONE_ADDR, dest_reg);
        word_to_write->type = REG_UNION_TYPE;

        code_img[(*ic) - IC_INIT_VALUE] = word_to_write;
    }
    else if(addr != DIRECT_ADDR){ /* if operand is a label we increment ic but don't write a word. label will be added in second_pass. */
        /* addressing type must be IMMEDIATE_ADDR */
        long value = strtol(operand + 1, &ptr, 10); /* operand is a number that starts with # */
        word_to_write = (machine_word *) malloc_with_check(sizeof(machine_word));
        word_to_write->length = 0; /* Not Code word! */
        (word_to_write->word).data = build_data_word(IMMEDIATE_ADDR, value, false);
        word_to_write->type = DATA_UNION_TYPE;

        code_img[(*ic) - IC_INIT_VALUE] = word_to_write;
    }
}
static void fpass_build_word_from_src_operand(line_info line, machine_word **code_img, long *ic, char *operand, addressing_type addr){
    machine_word *word_to_write;
    char *ptr;
    reg src_reg;

    if(addr == DIRECT_REGISTER_ADDR || addr == INDIRECT_REGISTER_ADDR){
        /* if the operands start with a star (INDIRECT_REGISTER_ADDR) we skip it and find the operand*/                
        src_reg = get_register_by_addr(operand, addr);

        word_to_write = (machine_word *) malloc_with_check(sizeof(machine_word));
        word_to_write->length = 0; /* not a codeword. */
        (word_to_write->word).reg = build_register_word(src_reg, NONE_ADDR);
        word_to_write->type = REG_UNION_TYPE;

        code_img[(*ic) - IC_INIT_VALUE] = word_to_write;
    }
    else if(addr != DIRECT_ADDR){ /* if operand is a label we increment ic but don't write a word. label will be added in second_pass. */
        /* addressing type must be IMMEDIATE_ADDR */
        long value = strtol(operand + 1, &ptr, 10); /* operand is a number that starts with # */
        word_to_write = (machine_word *) malloc_with_check(sizeof(machine_word));
        word_to_write->length = 0; /* Not Code word! */
        (word_to_write->word).data = build_data_word(IMMEDIATE_ADDR, value, false);
        word_to_write->type = DATA_UNION_TYPE;

        code_img[(*ic) - IC_INIT_VALUE] = word_to_write;
    }
}

static reg get_register_by_addr(const char *operand, addressing_type addressing) {
    /* if register starts with '*', we skip it. */
    return addressing == INDIRECT_REGISTER_ADDR ? get_register_by_name(operand + 1) : get_register_by_name(operand);
}