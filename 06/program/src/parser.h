#pragma once 

#define MAX_A_INST_VALUE_SIZE 100
#define MAX_LABEL_SIZE 100
#define MAX_C_INST_SIZE 100
#define MAX_C_INST_DEST_SIZE 30 
#define MAX_C_INST_COMP_SIZE 30 
#define MAX_C_INST_JUMP_SIZE 30  

enum OutputType{
    INSTRUCTION,
    LABEL,
    WHITESPACE,
    COMMENT
};

enum InstructionType{
    A,
    C
};

struct instruction{
    struct a_instruction *ainst;
    struct c_instruction *cinst;
    enum InstructionType type;
};

struct a_instruction{
    char *value;
};

struct c_instruction{
    char *dest;
    char *comp;
    char *jump;
};

struct label{
    char *name;
};

struct output{
    struct label *label;
    struct instruction *instruction;
    enum OutputType type;
};

struct output* parse(char *stmnt, size_t num);
void free_parse_out(struct output* parse_out);