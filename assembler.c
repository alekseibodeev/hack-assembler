/*
 * File: assembler.c
 * -----------------
 *  reads assembly language commands and translates them into appropriate
 *  binary hack encodings
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "code.h"
#include "helpers.h"
#include "parser.h"
#include "table.h"

/*
 * Function: resolve_label_symbols
 * -------------------------------
 *  goes through source code line by line and builts symbol table
 *
 *  input_stream: assembler language source file stream
 *  table: table to populate with labels
 */
static void resolve_label_symbols(FILE *input_stream, table_t *table)
{
    asm_command_t *command;
    short ln = -1; /* current instruction line */

    while ((command = get_command(input_stream))) {
        switch (command->type) {
            case A_COMMAND:
            case C_COMMAND:
                ln++;
                break;
            case L_COMMAND:
                table_add(table, command->symbol, ln + 1);
                break;
        }

        command_del(command);
    }
}

/*
 * Function: resolve_var_symbol
 * ----------------------------
 *  resolves symbol in an A command
 *
 *  symbol: variable or numeric symbol
 *  table: symbol table
 *  address: next available address
 *
 *  returns: integer value of the symbol
 */
static short resolve_var_symbol(const char *symbol,
        table_t *table, short *address_ptr)
{
    if (str_isnum(symbol)) {
        return atoi(symbol);
    }
    if (!table_contains(table, symbol)) {
        table_add(table, symbol, *address_ptr);
        *address_ptr += 1;
    }
    return table_get(table, symbol);
}

/*
 * Function: write_hack_command
 * ----------------------------
 *  writes 'code' to the stream as sequence of 0's and 1's
 *
 *  stream: writable stream
 *  code: hack command code represented as short int
 */
static void write_hack_command(FILE *stream, short code)
{
    int i;
    char binary[HACK_WORD_SIZE + 1]; /* add 1 for string terminator '\0' */

    for (i = 0; i < HACK_WORD_SIZE; i++) {
        /* extracts i-th most significat binary digit and
         * coverts it to ASCII char */
        binary[i] = ((code >> (HACK_WORD_SIZE - i - 1)) & 1) + '0';
    }

    binary[i] = '\0';
    fprintf(stream, "%s\n", binary);
}

/*
 * Function: write_a_command
 * -------------------------
 *  writes binary encoding of A command to stream
 *
 *  stream: writable stream
 *  command: assembler command structure
 */
static void write_a_command(FILE *stream,
        asm_command_t *command, table_t *table, short *address_ptr)
{
    short code = resolve_var_symbol(command->symbol, table, address_ptr);
    write_hack_command(stream, code);
}

/*
 * Function: write_c_command
 * -------------------------
 *  writes binary encoding of C command to stream
 *
 *  stream: writable stream
 *  command: assembler command structure
 */
static void write_c_command(FILE *stream, asm_command_t *command)
{
    short code = encode_command(command->dest, command->comp, command->jump);
    write_hack_command(stream, code);
}

/*
 * Function: write_help_msg
 * ------------------------
 *  writes help message for HackAssembler user
 */
static void write_help_msg(void)
{
    printf("\nUsage: HackAssembler source\n\n"
           "Assemble ASM source file.\n\n"
           "Arguments:\n"
           "source(required)\tsource file path (must have .asm suffix)\n\n");
}

/*
 * Function: init_builtins
 * -----------------------
 *  creates new table and populates it with hack assembler builtin symbols
 *
 *  returns: brand new table
 */
static table_t *init_builtins(void)
{
    table_t *table = table_new();

    table_add(table, "R0", 0);
    table_add(table, "R1", 1);
    table_add(table, "R2", 2);
    table_add(table, "R3", 3);
    table_add(table, "R4", 4);
    table_add(table, "R5", 5);
    table_add(table, "R6", 6);
    table_add(table, "R7", 7);
    table_add(table, "R8", 8);
    table_add(table, "R9", 9);
    table_add(table, "R10", 10);
    table_add(table, "R11", 11);
    table_add(table, "R12", 12);
    table_add(table, "R13", 13);
    table_add(table, "R14", 14);
    table_add(table, "R15", 15);

    table_add(table, "SP", 0);
    table_add(table, "LCL", 1);
    table_add(table, "ARG", 2);
    table_add(table, "THIS", 3);
    table_add(table, "THAT", 4);

    table_add(table, "SCREEN", 16384);
    table_add(table, "KBD", 24576);

    return table;
}

/*
 * Function: generate_hack_commands
 * --------------------------------
 *  parses source code line by line, resolves symbols and writes hack commands
 *
 *  input_stream: readable assembler commands stream
 *  output_stream: writable hack commands stream
 *  table: symbol table
 */
static void generate_hack_commands(FILE *input_stream,
        FILE *output_stream, table_t *table)
{
    asm_command_t *command;
    short address = FIRST_FREE_ADDRESS;

    while ((command = get_command(input_stream))) {
        switch (command->type) {
            case A_COMMAND:
                write_a_command(output_stream, command, table, &address);
                break;
            case C_COMMAND:
                write_c_command(output_stream, command);
                break;
            case L_COMMAND:
                break;
        }

        command_del(command);
    }
}

/*
 * Function: assemble
 * ------------------
 *  reads assembler commands for input stream and writes binary encodings
 *  to output stream
 *
 *  input_stream: data reader stream
 *  output_stream: data writer stream
 */
void assemble(FILE *input_stream, FILE *output_stream)
{
    /* initialize symbol table */
    table_t *table = init_builtins();

    /* first pass: build symbol table */
    resolve_label_symbols(input_stream, table);

    /* restore stream */
    rewind(input_stream);

    /* second pass: write actual code */
    generate_hack_commands(input_stream, output_stream, table);

    /* cleanup */
    table_del(table);
}

/*
 * Function: parse_args
 * --------------------
 *  parses CLI arguments and stores valid argument in source
 *  terminates program and writes help message if invalid arguments are passed
 *
 *  !!! user in charge of freeing stored arguments
 *
 *  argc: argument count
 *  argv: argument vector (list of arguments)
 *
 *  returns: parsed source argument
 */
char *parse_args(int argc, char **argv)
{
    if (argc != 2 || !str_ends_with(argv[1], INPUT_SUFFIX)) {
        write_help_msg();
        exit(1);
    }

    return strdup(argv[1]);
}

/*
 * Function: get_output
 * --------------------
 *  replaces input file extension with output file extension
 *
 *  source: input file path
 *
 *  returns: output file path
 */
char *get_output(const char *source)
{
    size_t prefix_len, suffix_len;
    char *source_suffix, *output;

    source_suffix = strrchr(source, '.');
    prefix_len = source_suffix - source;
    suffix_len = strlen(OUTPUT_SUFFIX);
    output = malloc(prefix_len + suffix_len + 1);

    strncpy(output, source, prefix_len);
    output[prefix_len] = '\0'; /* 'strncpy' doesn't put '\0', but it's needed
                                  for later usage of 'strcat' */
    strcat(output, OUTPUT_SUFFIX);
    return output;
}
