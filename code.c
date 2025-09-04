/*
 * File: code.c
 * ------------
 *  translates Hack Assembly language mnemonics into binary codes
 */

#include <string.h>

/*
 * Function: encode_dest
 * ---------------------
 *  translates 'dest' mnemonic into 3 bit binary mask
 *
 *  | dest  | d1  d2  d3 |
 *  |-------|------------|
 *  | null  |  0   0   0 |
 *  | M     |  0   0   1 |
 *  | D     |  0   1   0 |
 *  | MD    |  0   1   1 |
 *  | A     |  1   0   0 |
 *  | AM    |  1   0   1 |
 *  | AD    |  1   1   0 |
 *  | AMD   |  1   1   1 |
 *
 *  dest: symbolic 'dest' part of C command
 *
 *  returns: dest encoded as short int
 */
short encode_dest(const char *dest)
{
    short code = 0;

    if (!dest) {
        return code;
    }
    if (strchr(dest, 'M')) {
        code |= 1; /* 001 */
    }
    if (strchr(dest, 'D')) {
        code |= 2; /* 010 */
    }
    if (strchr(dest, 'A')) {
        code |= 4; /* 100 */
    }

    return code;
}

/* Function: encode_comp
 * ---------------------
 *  translates 'comp' mnemonic into 7 bit binary mask
 *
 *  | comp (a=0) | comp (a=1) | c1 c2 c3 c4 c5 c6 |
 *  |------------|------------|-------------------|
 *  | 0          |            |  1  0  1  0  1  0 |
 *  | 1          |            |  1  1  1  1  1  1 |
 *  | -1         |            |  1  1  1  0  1  0 |
 *  | D          |            |  0  0  1  1  0  0 |
 *  | A          | M          |  1  1  0  0  0  0 |
 *  | !D         |            |  0  0  1  1  0  1 |
 *  | !A         | !M         |  1  1  0  0  0  1 |
 *  | -D         |            |  0  0  1  1  1  1 |
 *  | -A         | -M         |  1  1  0  0  1  1 |
 *  | D+1        |            |  0  1  1  1  1  1 |
 *  | A+1        | M+1        |  1  1  0  1  1  1 |
 *  | D-1        |            |  0  0  1  1  1  0 |
 *  | A-1        | M-1        |  1  1  0  0  1  0 |
 *  | D+A        | D+M        |  0  0  0  0  1  0 |
 *  | D-A        | D-M        |  0  1  0  0  1  1 |
 *  | A-D        | M-D        |  0  0  0  1  1  1 |
 *  | D&A        | D&M        |  0  0  0  0  0  0 |
 *  | D|A        | D|M        |  0  1  0  1  0  1 |
 *
 *  comp: symbolic 'comp' part of C command
 *
 *  returns: comp encoded as short int
 */
short encode_comp(const char *comp)
{
    if (!strcmp(comp, "0")) {
        return 0x2A; /* 0010 1010 */
    } else if (!strcmp(comp, "1")) {
        return 0x3F; /* 0011 1111 */
    } else if (!strcmp(comp, "-1")) {
        return 0x3A; /* 0011 1010 */
    } else if (!strcmp(comp, "D")) {
        return 0x0C; /* 0000 1100 */
    } else if (!strcmp(comp, "A")) {
        return 0x30; /* 0011 0000 */
    } else if (!strcmp(comp, "M")) {
        return 0x70; /* 0111 0000 */
    } else if (!strcmp(comp, "!D")) {
        return 0x0D; /* 0000 1101 */
    } else if (!strcmp(comp, "!A")) {
        return 0x31; /* 0011 0001 */
    } else if (!strcmp(comp, "!M")) {
        return 0x71; /* 0111 0001 */
    } else if (!strcmp(comp, "-D")) {
        return 0x0F; /* 0000 1111 */
    } else if (!strcmp(comp, "-A")) {
        return 0x33; /* 0011 0011 */
    } else if (!strcmp(comp, "-M")) {
        return 0x73; /* 0111 0011 */
    } else if (!strcmp(comp, "D+1")) {
        return 0x1F; /* 0001 1111 */
    } else if (!strcmp(comp, "A+1")) {
        return 0x37; /* 0011 0111 */
    } else if (!strcmp(comp, "M+1")) {
        return 0x77; /* 0111 0111 */
    } else if (!strcmp(comp, "D-1")) {
        return 0x0E; /* 0000 1110 */
    } else if (!strcmp(comp, "A-1")) {
        return 0x32; /* 0011 0010 */
    } else if (!strcmp(comp, "M-1")) {
        return 0x72; /* 0111 0010 */
    } else if (!strcmp(comp, "D+A")) {
        return 0x02; /* 0000 0010 */
    } else if (!strcmp(comp, "D+M")) {
        return 0x42; /* 0100 0010 */
    } else if (!strcmp(comp, "D-A")) {
        return 0x13; /* 0001 0011 */
    } else if (!strcmp(comp, "D-M")) {
        return 0x53; /* 0101 0011 */
    } else if (!strcmp(comp, "A-D")) {
        return 0x07; /* 0000 0111 */
    } else if (!strcmp(comp, "M-D")) {
        return 0x47; /* 0100 0111 */
    } else if (!strcmp(comp, "D&A")) {
        return 0x00; /* 0000 0000 */
    } else if (!strcmp(comp, "D&M")) {
        return 0x40; /* 0100 0000 */
    } else if (!strcmp(comp, "D|A")) {
        return 0x15; /* 0001 0101 */
    }                      /* D|M */
    return 0x55; /* 0101 0101 */
}

/*
 * Function: encode_jump
 * ---------------------
 *  translates 'jump' mnemonic into 3 bit binary mask
 *
 *  | jump  | j1  j2  j3 |
 *  |-------|------------|
 *  | null  |  0   0   0 |
 *  | JGT   |  0   0   1 |
 *  | JEQ   |  0   1   0 |
 *  | JGE   |  0   1   1 |
 *  | JLT   |  1   0   0 |
 *  | JNE   |  1   0   1 |
 *  | JLE   |  1   1   0 |
 *  | JMP   |  1   1   1 |
 *
 *  jump: symbolic 'jump' part of C command
 *
 *  returns: jump encoded as short int
 */
short encode_jump(const char *jump)
{
    if (!jump) {
        return 0; /* 000 */
    }
    if (!strcmp(jump, "JGT")) {
        return 1; /* 001 */
    }
    if (!strcmp(jump, "JEQ")) {
        return 2; /* 010 */
    }
    if (!strcmp(jump, "JGE")) {
        return 3; /* 011 */
    }
    if (!strcmp(jump, "JLT")) {
        return 4; /* 100 */
    }
    if (!strcmp(jump, "JNE")) {
        return 5; /* 101 */
    }
    if (!strcmp(jump, "JLE")) {
        return 6; /* 110 */
    }               /* JMP */
    return 7; /* 111 */
}

/*
 * Function: encode_command
 * ------------------------
 *  translates C command mnemonics into 16 bit binary mask
 *
 *  C command in binary specified with the following form:
 *  1 1 1 a c1 c2 c3 c4 c5 c6 d1 d2 d3 j1 j2 j3
 *
 *  dest: symbolic 'dest' part of C command
 *  comp: symbolic 'comp' part of C command
 *  jump: symbolic 'jump' part of C command
 *
 *  returns: C command encoded as short int
 */
short encode_command(const char *dest, const char *comp, const char *jump)
{
    /* C command always has its 3 most significant bits set to '1'
     * so initial value should be 1110 0000 0000 0000 in binary */
    short code = 0xE000;

    /* bits representic 'comp' mnemonic have offset of 6
     * according to specification */
    code |= encode_comp(comp) << 6;

    /* bits representing 'dest' mnemonic have offset of 3
     * according to specification */
    code |= encode_dest(dest) << 3;

    /* 'jump' bits have no offset */
    code |= encode_jump(jump);

    return code;
}
