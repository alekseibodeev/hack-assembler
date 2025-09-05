/*
 * File: main.c
 * ------------
 *  entry point for hack assembler program
 */

#include <stdio.h>
#include <stdlib.h>

#include "assembler.h"

int main(int argc, char **argv)
{
    char *source, *output;
    FILE *input_stream, *output_stream;

    source = parse_args(argc, argv);
    output = get_output(source);

    input_stream = fopen(source, "r");
    output_stream = fopen(output, "w");

    assemble(input_stream, output_stream);

    /* cleanup */
    free(source);
    free(output);
    fclose(input_stream);
    fclose(output_stream);

    return 0;
}
