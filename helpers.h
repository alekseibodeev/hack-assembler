/*
 * File: helpers.h
 * ---------------
 *  function declarations for helper functions
 *
 *  set of utility functions
 */

#ifndef HACK_ASM_HELPERS_H
#define HACK_ASM_HELPERS_H

/*
 * Function: str_ends_with
 * -----------------------
 *  determines whether string ends with given suffix
 *
 *  s: string to be tested
 *  suffix: target suffix
 *
 *  returns: true if ends with suffix
 *           false otherwise
 */
bool str_ends_with(const char *s, const char *suffix);

/*
 * Function: str_isnum
 * -------------------
 *  checks wether the string is numeric
 *
 *  s: string to test
 *
 *  returns: true if 's' consists only of numeric characters
 *           false otherwise
 */
bool str_isnum(const char *s);

#endif // !HACK_ASM_HELPERS_H
