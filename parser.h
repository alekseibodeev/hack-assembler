/*
 * File: parser.h
 * --------------
 * constants and function declarations for parser module
 *
 * reads an assembly language commands and breaks them into underlying
 * components (fields and symbols)
 *
 * provides convenient access to the commands components
 *
 * in addition, removes all whitespace and comments
 */

#ifndef HACK_ASM_PARSER_H
#define HACK_ASM_PARSER_H

#include <stdlib.h>

#define MAXLINE 256

typedef enum {
    A_COMMAND,
    C_COMMAND,
    L_COMMAND
} command_type_t;

typedef struct {
    command_type_t type;
    char *symbol;
    char *dest;
    char *comp;
    char *jump;
} asm_command_t;

/*
 * Function: command_del
 * ---------------------
 *  frees memory allocated by command structure
 *
 *  command: a command to be deleted (assumes command is not NULL pointer)
 */
void command_del(asm_command_t *command);

/*
 * Function: get_command
 * ---------------------
 *  reads next command from the file stream and returns in a structured way
 *
 *  memory allocated for command have to be freed by user of the function
 *  with corresponding destructor 'command_del'
 *
 *  stream: input data stream to be read
 *
 *  returns: pointer to structure representing assembler command
 *           NULL if no commands left
 */
asm_command_t *get_command(FILE *stream);

#endif // !HACK_ASM_PARSER_H
