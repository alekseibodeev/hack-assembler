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
    char *s_suffix;
    size_t s_len = strlen(s);
    size_t suffix_len = strlen(suffix);

    if (suffix_len > s_len) {
        return false;
    }

    s_suffix = (char *) s;
    /* expression 's_suffix - s' produces prefix length
     * so 'len' - 'prefix len' = 'suffix len'
     * then compare length of current string suffix with the target one */
    while (s_len - (s_suffix - s) > suffix_len) {
        s_suffix++;
    }

    return !strcmp(s_suffix, suffix);
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
