#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "parser.h"
#include "translate.h"

struct symbol {
    char key[4];
    uint8_t value;
};

//Table for comp - value is a 8 bit integer MSB is 'a' bit and 6 digits in LSB is 'c' bit
//table for dest - value is a 8 bit integer. only 3 lsb is good.
//TABLE FOR jump - value is a 8 bit integer. only 3 lsb is good
unsigned const int comp_symbols_size = 28;
static struct symbol comp_symbols[] = {
    {.key = "0\0", .value = 42},
    {.key = "1\0", .value = 63},
    {.key = "-1\0", .value = 58},
    {.key = "D\0", .value = 12},
    {.key = "A\0", .value = 48},
    {.key = "M\0", .value = 176},
    {.key = "!D\0", .value = 13},
    {.key = "!A\0", .value = 49},
    {.key = "-D\0", .value = 15},
    {.key = "!M\0", .value = 177},
    {.key = "-A\0", .value = 51},
    {.key = "-M\0", .value = 179},
    {.key = "D+1\0", .value = 31},
    {.key = "A+1\0", .value = 55},
    {.key = "M+1\0", .value = 183},
    {.key = "D-1\0", .value = 14},
    {.key = "A-1\0", .value = 50},
    {.key = "M-1\0", .value = 178},
    {.key = "D+M\0", .value = 130},
    {.key = "D+A\0", .value = 2},
    {.key = "D-M\0", .value = 147},
    {.key = "D-A\0", .value = 19},
    {.key = "A-D\0", .value = 7},
    {.key = "M-D\0", .value = 135},
    {.key = "D&M\0", .value = 128},
    {.key = "D&A\0", .value = 0},
    {.key = "D|M\0", .value = 149},
    {.key = "D|A\0", .value = 21}
};

unsigned const int dest_symbols_size = 7;
static struct symbol dest_symbols[] = {
    {.key = "M\0", .value = 1},
    {.key = "D\0", .value = 2},
    {.key = "MD\0", .value = 3},
    {.key = "A\0", .value = 4},
    {.key = "AM\0", .value = 5},
    {.key = "AD\0", .value = 6},
    {.key = "AMD\0", .value = 7}
};

unsigned const int jump_symbols_size = 7;
static struct symbol jump_symbols[] = {
    {.key = "JGT\0", .value = 1},
    {.key = "JEQ\0", .value = 2},
    {.key = "JGE\0", .value = 3},
    {.key = "JLT\0", .value = 4},
    {.key = "JNE\0", .value = 5},
    {.key = "JLE\0", .value = 6},
    {.key = "JMP\0", .value = 7}
};

const uint8_t * find_symbol(struct symbol sym[], unsigned const int size, char *str);

uint16_t translate(struct output *statement)
{
    uint16_t ret = 0;
    
    switch (statement->type) {
        case INSTRUCTION:
            if (statement->instruction->type == A) {
                struct a_instruction *ainst = statement->instruction->ainst;
                ret = (uint16_t) strtol(ainst->value, NULL, 10);
            } else if (statement->instruction->type == C){
                struct c_instruction *cinst = statement->instruction->cinst;
                const uint8_t *comp = find_symbol(comp_symbols, comp_symbols_size, cinst->comp);
                const uint8_t *dest = find_symbol(dest_symbols, dest_symbols_size, cinst->dest);
                const uint8_t *jump = find_symbol(jump_symbols, jump_symbols_size, cinst->jump);
                ret |= (7 << 13); //c-inst
                ret |= (comp != NULL) ? ((*comp & 128) << 5) : 0;
                ret |= (comp != NULL) ? ((*comp & 63) << 6) : 0;
                uint16_t d = (dest != NULL) ? *dest : 0;
                uint16_t j = (jump != NULL) ? *jump : 0;
                
                ret |= (d << 3);
                ret |= j;
            }
            break;
        case LABEL:
            break;
        case WHITESPACE:
            break;
        case COMMENT:
            break;
    }
    
    return ret;
}

const uint8_t * find_symbol(struct symbol sym[], unsigned const int size, char *str)
{
    if (str != NULL) {
        for (int i=0; i<size; i++) {
            if (strcmp(sym[i].key, str) == 0) {
                return &(sym[i].value);
            }
        }
    }
    return NULL;
}