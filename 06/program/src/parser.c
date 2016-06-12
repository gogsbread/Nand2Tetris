#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "parser.h"

struct a_instruction* parse_a(const char *inst);
struct c_instruction* parse_c(const char *inst);
 
struct output* parse(char *stmnt, size_t num)
{
    struct output *result = (struct output *)malloc(sizeof(struct output));
    result->type = WHITESPACE;
    result->label = NULL;
    result->instruction = NULL;
    
    char *statement = (char *)malloc(sizeof(char)*(num+1));
    strncpy(statement, stmnt, num);
    statement[num] = '\0';
    
    const char *c = statement;
    while (*c != '\0' && isspace(*c)) {
        ++c;
    }
    if (*c != '\0') {
        result->instruction = (struct instruction *)malloc(sizeof(struct instruction)); 
        if (*c == '@') { //A-instruction
            result->type = INSTRUCTION;
            (result->instruction)->type = A;
            (result->instruction)->ainst = parse_a(c);
        } else if (*c == '(') { //label
            result->type = LABEL;
            ++c; //move away from ')'
            result->label = (struct label *)malloc(sizeof(struct label));
            (result->label)->name = (char *)malloc(sizeof(char) * MAX_LABEL_SIZE);
            size_t i = 0;
            while(c[i] != '\0' && c[i] != ')'){
               ((result->label)->name)[i] = c[i];
               ++i;
            }
        } else if (*c == '/' && *(++c) == '/') { //comment
            result->type = COMMENT;
        } else { //C-instruction
            result->type = INSTRUCTION;
            (result->instruction)->type = C;
            (result->instruction)->cinst = parse_c(c);
        }
    }
    
    free(statement);
    
    return result;
}

void free_parse_out(struct output *parse_out)
{
    if (parse_out->type == LABEL) {
        free((parse_out->label)->name);
        free(parse_out->label);    
    } else if (parse_out->type == INSTRUCTION) {
        if ((parse_out->instruction)->type == A) {
            free(((parse_out->instruction)->ainst)->value);
            free((parse_out->instruction)->ainst);
        } else if ((parse_out->instruction)->type == C) {
            free(((parse_out->instruction)->cinst)->dest);
            free(((parse_out->instruction)->cinst)->comp);
            free(((parse_out->instruction)->cinst)->jump);
            free((parse_out->instruction)->cinst);
        }
        free(parse_out->instruction);
    }
    free(parse_out);
}

struct a_instruction* parse_a(const char *inst)
{
    struct a_instruction *ainst = (struct a_instruction *)malloc(sizeof(struct a_instruction));
    ainst->value = (char *)malloc(sizeof(char) * MAX_A_INST_VALUE_SIZE);
    
    int8_t i = 1;//since the value starts at @
    while (!isspace(inst[i]) && i < MAX_A_INST_VALUE_SIZE) {
        (ainst->value)[i-1] = inst[i];
        ++i;
    }
    if (i == MAX_A_INST_VALUE_SIZE) { //LOG
        printf("LOG: The instruction size was greater than %d", MAX_A_INST_VALUE_SIZE);
    }
    
    return ainst; 
}

struct c_instruction* parse_c(const char *inst)
{
    struct c_instruction *cinst = (struct c_instruction *)malloc(sizeof(struct c_instruction));
    cinst->dest = NULL;
    cinst->comp = NULL;
    cinst->jump = NULL;
    
    int8_t i = 0;
    int8_t start = 0;
    typedef enum { DEST, COMP, JUMP} mode;
    mode m = DEST;
    while (!isspace(inst[i]) && i < MAX_C_INST_SIZE) {
        if (inst[i] == '=') { //we completed dest
            cinst->dest = (char *)malloc(sizeof(char) * (i-start)+1);
            strncpy(cinst->dest, (inst+start), i-start);
            cinst->dest[i-start] = '\0';
            start = i+1;
            m = COMP;
        } else if (inst[i] == ';') { //we completed comp
            cinst->comp = (char *)malloc(sizeof(char) * (i-start)+1);
            strncpy(cinst->comp, (inst+start), i-start);
            cinst->comp[i-start] = '\0';
            start = i+1;
            m = JUMP;
        }
        ++i;
    }
    if (m == COMP) { //possible if there was no jump
        cinst->comp = (char *)malloc(sizeof(char) * (i-start)+1);
        strncpy(cinst->comp, (inst+start), i-start);
    } else if (m == JUMP){
        cinst->jump = (char *)malloc(sizeof(char) * (i-start)+1);
        strncpy(cinst->jump, (inst+start), i-start);
    }
    
    return cinst; 
}