/*
 * File: assembler.h
 * -----------------
 *  constants and function declarations for assembler module
 *
 *  reads assembly language commands and translates them into appropriate
 *  binary hack encodings
 */

#ifndef HACK_ASSEMBLER_H
#define HACK_ASSEMBLER_H

#define HACK_WORD_SIZE 16
#define FIRST_FREE_ADDRESS 16
#define INPUT_SUFFIX ".asm"
#define OUTPUT_SUFFIX ".hack"

/*
 * Function: assemble
 * ------------------
 *  reads assembler commands for input stream and writes binary encodings
 *  to output stream
 *
 *  input_stream: data reader stream
 *  output_stream: data writer stream
 */
void assemble(FILE *input_stream, FILE *output_stream);

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
char *parse_args(int argc, char **argv);

/*
 * Function: get_output_name
 * -------------------------
 *  replaces input file extension with output file extension
 *
 *  source: input file path
 *
 *  returns: output file path
 */
char *get_output(const char *source);

#endif // !HACK_ASSEMBLER_H
