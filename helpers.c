/*
 * File: helpers.c
 * ---------------
 *  set of utility functions
 */

#include <ctype.h>
#include <stdbool.h>
#include <string.h>

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
bool str_ends_with(const char *s, const char *suffix)
{
    size_t s_len = strlen(s);
    size_t suffix_len = strlen(suffix);

    if (suffix_len > s_len) {
        return false;
    }

    /* pointer arithmetic here extracts window with the size of taget suffix */
    return !strcmp(s + s_len - suffix_len, suffix);
}

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
bool str_isnum(const char *s)
{
    for (char *p = (char *) s; *p; p++) {
        if (!isdigit(*p)) {
            return false;
        }
    }

    return true;
}
