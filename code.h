/*
 * File: code.h
 * ------------
 *  function declarations for code module
 *
 *  translates Hack Assembly language mnemonics into binary codes
 */

#ifndef HACK_ASM_CODE_H
#define HACK_ASM_CODE_H

/*
 * Function: encode_dest
 * ---------------------
 *  translates 'dest' mnemonic into 3 bit binary mask
 *
 *  dest: symbolic 'dest' part of C command
 *
 *  returns: dest encoded as short int
 */
short encode_dest(const char *dest);

/* Function: encode_comp
 * ---------------------
 *  translates 'comp' mnemonic into 7 bit binary mask
 *
 *  comp: symbolic 'comp' part of C command
 *
 *  returns: comp encoded as short int
 */
short encode_comp(const char *comp);

/*
 * Function: encode_jump
 * ---------------------
 *  translates 'jump' mnemonic into 3 bit binary mask
 *
 *  jump: symbolic 'jump' part of C command
 *
 *  returns: jump encoded as short int
 */
short encode_jump(const char *jump);

/*
 * Function: encode_command
 * ------------------------
 *  translates C command mnemonics into 16 bit binary mask
 *
 *  dest: symbolic 'dest' part of C command
 *  comp: symbolic 'comp' part of C command
 *  jump: symbolic 'jump' part of C command
 *
 *  returns: C command encoded as short int
 */
short encode_command(const char *dest, const char *comp, const char *jump);

#endif // !HACK_ASM_CODE_H
