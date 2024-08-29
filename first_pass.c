#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "first_pass.h"
#include "utils.h"
#include "globals.h"
#include "instructions.h"
#include "code.h"



bool fpass_process_line(line_info line, machine_word **code_img, long *data_img, long *ic, long *dc, table *symbol_table){
    bool label_found = false;
    int i,j, length;
    char symbol[MAX_LINE_LENGTH] = {0}, tokenized_line[MAX_LINE_LENGTH + 2], *first_word, *token;
    instruction inst;

    strcpy(tokenized_line, line.content);

    first_word = strtok(tokenized_line, " \t\n"); //discluding label.

    if(first_word == NULL || first_word[0] == '\n' || first_word[0] == EOF || first_word[0] == ';'){
        // the line is empty or a comment
        return true;
    }

    //check if label

    length = strlen(first_word);
    if(length > 0 && first_word[length-1] == ':'){
        // possible label found!

        first_word[length - 1] = '\0'; // remove the ':' from the label


        
        strcpy(symbol, first_word);

        if (!is_valid_label_name(symbol)) {
            printf_line_error(line, "illegal label name.");
            return false;
        }
        first_word = strtok(NULL, " \t\n");
        
        label_found = true;
    }

    if(key_exists_in_table(symbol_table, symbol)){
        printf_line_error(line, "label name already exists.");
        return false;
    } 

    inst = get_instruction_by_name(first_word);

    if(inst != NONE_INST){
        if(label_found && (inst == DATA_INST || inst == STRING_INST)){
            add_table_item(symbol_table, symbol, *dc, DATA_SYMBOL);
        }
        if(inst = DATA_INST){
            return handle_data_instruction(line, dc, symbol_table);
        }
        if(inst == STRING_INST){
            return handle_string_instruction(line, dc, symbol_table);
        }
        if(inst == EXTERN_INST){
            token = strtok(NULL, " \t\n");
            strcpy(symbol, token);

            if(!is_valid_label_name(symbol, line)){
                printf_line_error(line, "illegal label name.");
                return false;
            }
            if(!key_exists_in_table(symbol_table, symbol)){
                printf_line_error(line, "label name already exists.");
                return false;
            }
            if(strtok(NULL, " \t\n") != NULL){
                printf_line_error(line, "more than one arguement was given to .extern instruction.");
            }

            add_table_item(symbol_table, symbol, 0, EXTERNAL_SYMBOL); /* Extern value is defaulted to 0 */
            return true;
        }
    }
    else{ //only option left to check is a command
        if(label_found){
            add_table_item(symbol_table, symbol, *ic, CODE_SYMBOL);
        }
        //handle code stuff.
    }
}

bool handle_code_line(line_info line, long *ic, machine_word **code_img){
    char op_name[8], operands[2][MAX_LINE_LENGTH] = {0};
    opcode curr_opcode;
    code_word *codeword;
    long ic_before;
    int op_count, index, i;
    machine_word *word_to_write;

    SKIP_WHITE_SPACES(line.content, index);
    i = 0;
    while(line.content[index] != '\0' && !isspace(line.content[index])){
        op_name[i++] = line.content[index++];
    } //extract the opcode name

    curr_opcode = get_opcode_by_name(op_name);
    if(curr_opcode == NONE_OP){
        printf_line_error(line, "unrecognized command.");
        return false;
    }

    if(!extract_operands(line, index, &op_count, operands)){
        return false;
    }

    if ((codeword = get_code_word(line, curr_opcode, op_count, operands)) == NULL) {
		return false;
	}

    //remember the position of the codeword. 
    ic_before = *ic;

    word_to_write = (machine_word *) malloc_with_check(sizeof(machine_word));
	(word_to_write->word).code = codeword;
	code_img[(*ic) - IC_INIT_VALUE] = word_to_write;

    if (op_count) { /* If there's at least 1 operand */
		build_extra_codewords_fpass(code_img, ic, operands, op_count);
	}

    code_img[ic_before - IC_INIT_VALUE]->length = (*ic) - ic_before;

	return true; /* No errors */
}


static void build_extra_codewords_fpass(machine_word **code_img, long *ic, char **operands, int op_count) {
    char *ptr;
	machine_word *word_to_write;

	addressing_type first_addressing = op_count >= 1 ? get_addressing_type(operands[0]) : NONE_ADDR;
    addressing_type second_addressing = op_count == 2 ? get_addressing_type(operands[0]) : NONE_ADDR;
	
    //handle both operands together.
    if((first_addressing == DIRECT_REGISTER_ADDR || first_addressing == INDIRECT_REGISTER_ADDR) &&
                        second_addressing == DIRECT_REGISTER_ADDR || second_addressing == INDIRECT_REGISTER_ADDR){ // check if both addressing types are registers.
        (*ic)++;

        word_to_write = (machine_word *) malloc_with_check(sizeof(machine_word));
        word_to_write->length = 0; //not a codeword.
        (word_to_write->word).reg = build_register_word(first_addressing, second_addressing);

        code_img[(*ic) - IC_INIT_VALUE] = word_to_write;
        return;
    }

    //handle each operand seperately. starting with first operand.
    if(first_addressing == NONE_ADDR){
        return;
    }

    (*ic)++;
    if(first_addressing == DIRECT_REGISTER_ADDR || first_addressing == INDIRECT_REGISTER_ADDR){
        word_to_write = (machine_word *) malloc_with_check(sizeof(machine_word));
        word_to_write->length = 0; //not a codeword.
        (word_to_write->word).reg = build_register_word(first_addressing, NONE_ADDR);

        code_img[(*ic) - IC_INIT_VALUE] = word_to_write;
    }
    else if(first_addressing != DIRECT_ADDR){ // if operand is a label we increment ic but don't write a word. label will be added in second_pass.
        // addressing type must be IMMEDIATE_ADDR
        long value = strtol(operands[0] + 1, &ptr, 10); //operand is a number that starts with #
        word_to_write = (machine_word *) malloc_with_check(sizeof(machine_word));
        word_to_write->length = 0; /* Not Code word! */
        (word_to_write->word).data = build_data_word(IMMEDIATE_ADDR, value, false);

        code_img[(*ic) - IC_INIT_VALUE] = word_to_write;
    }

    if(second_addressing == NONE_ADDR){
        return;
    }

    (*ic)++;
    if(second_addressing == DIRECT_REGISTER_ADDR || second_addressing == INDIRECT_REGISTER_ADDR){
        word_to_write = (machine_word *) malloc_with_check(sizeof(machine_word));
        word_to_write->length = 0; //not a codeword.
        (word_to_write->word).reg = build_register_word(NONE_ADDR, second_addressing);

        code_img[(*ic) - IC_INIT_VALUE] = word_to_write;
    }
    else if(second_addressing != DIRECT_ADDR){ // if operand is a label we increment ic but don't write a word. label will be added in second_pass.
        // addressing type must be IMMEDIATE_ADDR
        long value = strtol(operands[1] + 1, &ptr, 10); //operand is a number that starts with #
        word_to_write = (machine_word *) malloc_with_check(sizeof(machine_word));
        word_to_write->length = 0; /* Not Code word! */
        (word_to_write->word).data = build_data_word(IMMEDIATE_ADDR, value, false);

        code_img[(*ic) - IC_INIT_VALUE] = word_to_write;
    }
}