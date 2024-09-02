#include <stdlib.h>
#include <string.h>
#include "second_pass.h"
#include "utils.h"
#include "instructions.h"
#include "code.h"


bool spass_process_line(line_info line, machine_word **code_img, long *ic, table *symbol_table){
    char symbol[MAX_LINE_LENGTH] = {0}, tokenized_line[MAX_LINE_LENGTH + 2];
    char *first_word, token;
    int length;
    instruction inst;
    table_entry *entry;

    strcpy(tokenized_line, line.content);

    first_word = strtok(tokenized_line, " \t\n"); //discluding label.

    if(first_word == NULL || first_word[0] == '\n' || first_word[0] == EOF || first_word[0] == ';'){
        // the line is empty or a comment
        return true;
    }

    //check if label

    length = strlen(first_word);
    if(length > 0 && first_word[length-1] == ':'){
        // if the line is a definition of a label we skip it.
        return true;
    }

    inst = get_instruction_from_word(first_word);

    if(inst == ENTRY_INST){
        token = strtok(NULL, " \t\n"); //get the label's name.
        if (token == NULL) {
            printf_line_error(line, "You have to specify a label name for .entry instruction.");
            return false;
        }

        if(!is_valid_label_name(token)){
            printf_line_error(line, "illegal label name.");
            return false;
        }

        /* if label is already marked as entry, ignore. */
        if(find_by_types(symbol_table, token, 1, ENTRY_SYMBOL) != NULL){
            return true;
        }

        // label could not be extern and entry at the same time.
        if(entry = find_by_types(symbol_table, token, 1, EXTERNAL_SYMBOL) != NULL){
            printf_line_error(line, "The symbol %s can be either external or entry, but not both.", token);
            return false;
        }

        if(entry = find_by_types(symbol_table, token, 2, CODE_SYMBOL, DATA_SYMBOL) == NULL){
            printf_line_error(line, "The symbol %s for .entry is undefined.", token);
            return false;
        }

        add_table_item(symbol_table, token, entry->value, ENTRY_SYMBOL);

        return true;
    }
    if(inst == NONE_INST){
        return add_symbols_to_code(line, ic, code_img, symbol_table);
    }
    return true; // returns true if it's an instruction differnt than entry.
}
bool add_symbols_to_code(line_info line, long *ic, machine_word **code_img, table *symbol_table){
    char operands[2][MAX_LINE_LENGTH];
    long curr_ic = ic;
    int index, length, op_count;
    bool line_valid;
    
    length = code_img[(*ic)]->length;

    if(length > 1){
        SKIP_WHITE_SPACES(line.content, index);
        while(line.content[index] != '\0' && !isspace(line.content[index])){
            index++;
        } //skip the opcode.

        extract_operands(line, index, &op_count, operands); //this will always return true, else the first_pass wouldn't be successful.

        if(op_count){
            line_valid = spass_process_operands(line, &curr_ic, operands, op_count, code_img, symbol_table);
        }
    }

    (*ic) += length;
    return true;
}


static bool spass_process_operands(line_info line, long *curr_ic, char **operands, int op_count,
                                     machine_word **code_img, table *symbol_table){
    machine_word *first_word_to_write, *second_word_to_write;
    table_entry *entry;
    long data;

    addressing_type first_addressing = op_count >= 1 ? get_addressing_type(operands[0]) : NONE_ADDR;
    addressing_type second_addressing = op_count == 2 ? get_addressing_type(operands[0]) : NONE_ADDR;

    //if both the operands are register type addressing they are both entered in the same code line so we increment the ic only once.
    if((first_addressing == DIRECT_REGISTER_ADDR || first_addressing == INDIRECT_REGISTER_ADDR) &&
                        second_addressing == DIRECT_REGISTER_ADDR || second_addressing == INDIRECT_REGISTER_ADDR){ 
        // check if both addressing types are registers.
        (*curr_ic)++;
        return true;
    }

    if(first_addressing == NONE_ADDR){
        return true;
    }

    (*curr_ic)++;
    if(first_addressing == DIRECT_ADDR){ //label
        if(entry = find_by_types(symbol_table, operands[0], 3, CODE_SYMBOL, DATA_SYMBOL, EXTERNAL_SYMBOL) == NULL){
            printf_line_error(line, "the symbol %s is not found", operands[0]);
        }
        data = entry->value;

        if(entry->type = EXTERNAL_SYMBOL){
            add_table_item(symbol_table, operands[0], curr_ic, EXTERNAL_REFERENCE);
        }

        first_word_to_write = (machine_word *) malloc_with_check(sizeof(machine_word));
        first_word_to_write->length = 0; //not a code word.
        (first_word_to_write->word).data = build_data_word(first_addressing, data, entry->type = EXTERNAL_SYMBOL);
    }
    //first operand done, now check if there's a second operand and process it.
    
    if(second_addressing == NONE_ADDR){
        return true;
    }

    (*curr_ic)++;
    if(second_addressing == DIRECT_ADDR){ //label
        if(entry = find_by_types(symbol_table, operands[1], 3, CODE_SYMBOL, DATA_SYMBOL, EXTERNAL_SYMBOL) == NULL){
            printf_line_error(line, "the symbol %s is not found", operands[1]);
        }
        data = entry->value;

        if(entry->type = EXTERNAL_SYMBOL){
            add_table_item(symbol_table, operands[1], curr_ic, EXTERNAL_REFERENCE);
        }

        second_word_to_write = (machine_word *) malloc_with_check(sizeof(machine_word));
        second_word_to_write->length = 0; //not a code word.
        (second_word_to_write->word).data = build_data_word(second_addressing, data, entry->type = EXTERNAL_SYMBOL);
    }
    
    return true;
}