/*
 * File: parser.c
 * --------------
 * reads an assembly language commands and breaks them into underlying
 * components (fields and symbols)
 *
 * provides convenient access to the commands components
 *
 * in addition, removes all whitespace and comments
 */

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

/*
 * Function: command_new
 * ---------------------
 *  assembler command struct constructor
 *
 *  type: type of assembler command
 *
 *  returns: a pointer to newly created command in memory
 */
static asm_command_t *command_new(command_type_t type)
{
    asm_command_t *command = malloc(sizeof(asm_command_t));
    command->type = type;
    command->symbol = NULL;
    command->dest = NULL;
    command->comp = NULL;
    command->jump = NULL;
    return command;
}

/*
 * Function: command_del
 * ---------------------
 *  frees memory allocated by command structure
 *
 *  command: a command to be deleted (assumes command is not NULL pointer)
 */
void command_del(asm_command_t *command)
{
    free(command->symbol);
    free(command->dest);
    free(command->comp);
    free(command->jump);
    free(command);
}

/*
 * Function: command_addr_new
 * --------------------------
 *  creates new command of type A
 *
 *  symbol: value (label or constant) associated with A command
 *          should be allocated
 *
 *  returns: command A structure
 */
static asm_command_t *command_addr_new(char *symbol)
{
    asm_command_t *command = command_new(A_COMMAND);
    command->symbol = symbol;
    return command;
}

/*
 * Function: command_comp_new
 * --------------------------
 *  creates new command of type C (assignment or jump one)
 *
 *  dest: symbolic dest part of the command (can be NULL), should be allocated
 *  comp: symbolic comp part of the command, should be allocated
 *  jump: symbolic jump part of the command (can be NULL), should be allocated
 *
 *  returns: command C structure
 */
static asm_command_t *command_comp_new(char *dest, char *comp, char *jump)
{
    asm_command_t *command = command_new(C_COMMAND);
    command->dest = dest;
    command->comp = comp;
    command->jump = jump;
    return command;
}

/*
 * Function: command_labl_new
 * --------------------------
 *  creates new command of type L
 *
 *  symbol: value associated with symbolic label, should be allocated
 *
 *  returns: command L structure
 */
static asm_command_t *command_labl_new(char *symbol)
{
    asm_command_t *command = command_new(L_COMMAND);
    command->symbol = symbol;
    return command;
}

/*
 * Function: iscomment
 * -------------------
 *  determines whether the given character is a start of the commented line
 *
 *  c: character to test agains
 *
 *  returns: true  - if it is a comment
 *           false - otherwise
 */
static bool iscomment(char c)
{
    /* first ver of assembler assumes the input is always valid
     * so 1 char is enough */
    return c == '/';
}

/*
 * Function: skip_comments_and_spaces
 * -----------------------------------------
 *  skips all leading whitespaces and comments until next instruction is found
 *  or end of the file is reached
 *
 *  stream: input stream with asm commands
 */
static void skip_comments_and_spaces(FILE *stream)
{
    char c = getc(stream);

    while (isspace(c) || iscomment(c)) {
        /* skip all leading whitespace */
        while (isspace(c)) {
            c = getc(stream);
        }

        if (iscomment(c)) {
            /* skip commented line until new line or end of file */
            while (c != EOF && c != '\n') {
                c = getc(stream);
            }
        }
    }

    /* return beginning of next assembler command back to stream */
    if (c != EOF) {
        ungetc(c, stream);
    }
}

/*
 * Function: read_command
 * -----------------------------
 *  reads next command from the input stream ignoring all spaces and comments
 *  assumes the buffer has enough space to store data
 *
 *  stream: data stream to be read from
 *  buffer: string to store assembler command
 *
 *  returns: amount of characters written to buffer
 */
static size_t read_command(FILE *stream, char *buffer)
{
    char c;
    size_t i;

    skip_comments_and_spaces(stream);

    i = 0;
    while ((c = getc(stream)) != EOF && c != '\n') {
        if (!isspace(c)) {
            buffer[i] = c;
            i++;
        }
    }

    buffer[i] = '\0';
    return i;
}

/*
 * Function: isaddr
 * ----------------
 *  determines whether the given command is of A type
 *
 *  line: assembler command in plain text
 *
 *  returns: true if command A
 *           false otherwise
 */
static bool isaddr(const char *line) {
    return line[0] == '@';
}

/*
 * Function: islabl
 * ----------------
 *  determines whether the given command is of type L
 *
 *  line: assembler command in plain text
 *
 *  returns: true if command L
 *           false otherwise
 */
static bool islabl(const char *line) {
    return line[0] == '(';
}

/*
 * Function: classify_command
 * --------------------------
 *  determines type of the command
 *
 *  line: assembler command in plain text
 *
 *  return: enum value associated with particular assembler command
 */
static command_type_t classify_command(const char *line)
{
    if (isaddr(line)) {
        return A_COMMAND;
    } else if (islabl(line)) {
        return L_COMMAND;
    }
    /* first ver assumes input commands are valid so don't need to check */
    return C_COMMAND;
}

/*
 * Function: get_symbol_from_addr
 * ------------------------------
 *  extracts symbolic value from command A string
 *
 *  line: assembler command in plain text
 *
 *  returns: symbolic value of command (label or constant)
 */
static char *get_symbol_from_addr(const char *line)
{
    /* copy whole string except first '@' char */
    return strdup(line + 1);
}

/*
 * Function: get_symbol_from_labl
 * ------------------------------
 *  extracts symbolic value of command L string
 *
 *  line: assembler command in plain text
 *
 *  returns: symbolic value of associated with command L
 */
static char *get_symbol_from_labl(const char *line, size_t line_len)
{
    /* copy all content except opening '(' and closing ')' chars */
    return strndup(line + 1, line_len - 2);
}

/*
 * Function: get_dest
 * ------------------
 *  extracts dest mnemonic of command C
 *
 *  line: assembler command in plain text
 *
 *  returns: dest part of the command
 *           NULL if the command doesn't have dest part
 */
static char *get_dest(const char *line)
{
    /* C command can be either 1: 'dest=comp' or 2: 'comp;jump' */
    char *divider = strchr(line, '=');

    /* second case - dest is not present */
    if (!divider) {
        return NULL;
    }

    /* copy all chars until '='
     * pointer arithmetic 'divider - line' produces length of dest part */
    return strndup(line, divider - line);
}

/*
 * Function: get_comp
 * ------------------
 *  extracts comp mnemonic of command C
 *
 *  line: assembler command in plain text
 *
 *  returns: comp part of the command
 */
static char *get_comp(const char *line)
{
    /* C command can be either 1: 'dest=comp' or 2: 'comp;jump' */
    char *divider = strchr(line, '=');

    /* first case - comp is on the right */
    if (divider) {
        return strdup(divider + 1);
    }

    /* second case - comp is on the left */
    divider = strchr(line, ';');

    /* pointer arithemtic produces length of comp part (before divider) */
    return strndup(line, divider - line);
}

/*
 * Function: get_jump
 * ------------------
 *  extracts jump mnemonic of command C
 *
 *  line: assembler command in plain text
 *
 *  returns: jump part of the command
 *           NULL if the command doesn't have jump part
 */
static char *get_jump(const char *line)
{
    /* C command can be either 1: 'dest=comp' or 2: 'comp;jump' */
    char *divider = strchr(line, ';');

    /* first case - jump is not present */
    if (!divider) {
        return NULL;
    }

    /* copy string after divider */
    return strdup(divider + 1);
}

/*
 * Function: get_command
 * ---------------------
 *  reads next command from the file stream and returns in a structured way
 *
 *  stream: input data stream to be read
 *
 *  returns: pointer to structure representing assembler command
 *           NULL if no commands left
 */
asm_command_t *get_command(FILE *stream)
{
    char line[MAXLINE];
    size_t line_len;
    char *symbol, *dest, *comp, *jump;
    asm_command_t *command;

    if ((line_len = read_command(stream, line)) == 0) {
        return NULL;
    }

    switch (classify_command(line)) {
        case A_COMMAND:
            symbol = get_symbol_from_addr(line);
            command = command_addr_new(symbol);
            break;
        case C_COMMAND:
            dest = get_dest(line);
            comp = get_comp(line);
            jump = get_jump(line);
            command = command_comp_new(dest, comp, jump);
            break;
        case L_COMMAND:
            symbol = get_symbol_from_labl(line, line_len);
            command = command_labl_new(symbol);
            break;
    }

    return command;
}
