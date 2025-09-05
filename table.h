/*
 * File: table.h
 * -------------
 *  types, constants and function declarations for symbol table module
 *
 *  dictionary ADT for storing symbol-address mappings
 */

#ifndef HACK_ASM_TABLE_H
#define HACK_ASM_TABLE_H

#include <stdbool.h>

#define TABLE_BUCKETS_N 127

typedef struct table_node_t {
    char *key;
    short val;
    struct table_node_t *next;
} table_node_t;

typedef struct {
    table_node_t *data[TABLE_BUCKETS_N];
    size_t size;
} table_t;

/* Function: table_new
 * -------------------
 *  create new symbol table
 *
 *  returns: pointer to allocated empty table
 */
table_t *table_new(void);

/*
 * Function: table_del
 * -------------------
 *  destroys symbol table
 *
 *  table: table to be deleted
 */
void table_del(table_t *table);

/*
 * Function: table_add
 * -------------------
 *  adds new entry to the table (assumes table don't have duplicates)
 *
 *  table: table to write to
 *  symbol: key of new entry
 *  address: value of new entry
 */
void table_add(table_t *table, const char *symbol, short address);

/*
 * Function: table_contains
 * ------------------------
 *  checks wether the given table has provided symbol
 *
 *  table: table to search in
 *  symbol: target key
 *
 *  returns: true if symbol is presented in the table
 *           false otherwise
 */
bool table_contains(table_t *table, const char *symbol);

/*
 * Function: table_get
 * -------------------
 *  searches for the address associated with the given symbol
 *
 *  table: table to search in
 *  symbol: target symbol
 *
 *  returns: address associated with symbol key
 *           -1 if symbol is not found
 */
short table_get(table_t *table, const char *symbol);

#endif // !HACK_ASM_TABLE_H
