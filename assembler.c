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
static void write_a_command(FILE *stream, asm_command_t *command)
{
    short code = atoi(command->symbol);
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
    asm_command_t *command;

    while ((command = get_command(input_stream))) {
        switch (command->type) {
            case A_COMMAND:
                write_a_command(output_stream, command);
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
