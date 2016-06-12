#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "parser.h"
#include "translate.h"

#define MAX_PATH_SIZE 256

void print_usage();

int main(int argc, char *argv[])
{
    if (argc != 2) {
        print_usage();
        return EXIT_FAILURE;
    }
    char *source_file_name = NULL;
    char dest_file_name[MAX_PATH_SIZE];
    memset(dest_file_name, '\0', MAX_PATH_SIZE);
    source_file_name = argv[1];
    if (argc >= 3) {
        strncpy(dest_file_name, argv[2], MAX_PATH_SIZE);
    } else {
        char *found = strrchr(source_file_name, '.');
        if (found != NULL) {
            char copy[found-source_file_name+1];
            memset(copy, '\0', found-source_file_name+1);
            strncpy(copy, source_file_name, found-source_file_name);
            snprintf(dest_file_name, MAX_PATH_SIZE, "%s.hack",copy);
        } else {
            snprintf(dest_file_name, MAX_PATH_SIZE, "%s.hack",source_file_name);
        }
    }
    
    FILE *sfp = fopen(source_file_name, "r");
    if (sfp == NULL) {
        printf("Error reading file\n");
        return EXIT_FAILURE;
    }
    printf("Writing to: %s\n",dest_file_name);
    FILE *dfp = fopen(dest_file_name, "w");
    if (dfp == NULL) {
        printf("Error writing file\n");
        return EXIT_FAILURE;
    }
    char instruction[100];
    while (fgets(instruction, sizeof(instruction), sfp) != NULL) {
        struct output *parse_out = parse(instruction, strlen(instruction));
        if (parse_out->type == INSTRUCTION) {
            uint16_t output = translate(parse_out);
            for (int i=15; i>=0; i--) {
                if ((output >> i) & 1) {
                    fputs("1", dfp);
                } else {
                    fputs("0", dfp);
                }
            }
            fputs("\n", dfp);
        }
        free_parse_out(parse_out);
    }
    fclose(dfp);
    fclose(sfp);
    
    return EXIT_SUCCESS;
}

void print_usage()
{
    printf("Error!!\nUsage: assembler <source> [dest]\n");
}