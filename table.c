/*
 * File: table.c
 * -------------
 *  dictionary ADT for storing symbol-address mappings
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "table.h"

/*
 * Function: hash
 * --------------
 *  hashing function for string keys
 *
 *  s: key to hash
 *
 *  returns: integer hash code of the given key
 */
static int hash(const char *s)
{
    int h = 0;
    int a = 31415;
    int b = 27831;

    for (char *p = (char *) s; *p; p++) {
        h = (a * h + *p) % TABLE_BUCKETS_N;
        a = a * b % (TABLE_BUCKETS_N - 1);
    }

    return h;
}

/*
 * Function: node_new
 * ------------------
 *  creates new table entry
 *
 *  key: key associated with table entry
 *  val: value associated with key
 *  next_ptr: pointer to the next table entry
 *
 *  returns: pointer to newly allocated table entry
 */
static table_node_t *node_new(const char *key,
        short val, table_node_t *next_ptr)
{
    table_node_t *node = malloc(sizeof(node));
    node->key = strdup(key);
    node->val = val;
    node->next = next_ptr;
    return node;
}

/*
 * Function: node_del
 * ------------------
 *  frees memory from the given node
 *
 *  node: node to be deleted
 */
static void node_del(table_node_t *node)
{
    free(node->key);
    free(node);
}

/* Function: table_new
 * -------------------
 *  create new symbol table
 *
 *  returns: pointer to allocated empty table
 */
table_t *table_new(void)
{
    table_t *table = malloc(sizeof(table_t));
    table->size = 0;

    for (int i = 0; i < TABLE_BUCKETS_N; i++) {
        table->data[i] = NULL;
    }

    return table;
}

/*
 * Function: table_del
 * -------------------
 *  destroys symbol table
 *
 *  table: table to be deleted
 */
void table_del(table_t *table)
{
    table_node_t *p, *q;

    for (int i = 0; i < TABLE_BUCKETS_N; i++) {
        for (p = table->data[i]; p; p = q) {
            q = p->next;
            node_del(p);
        }
    }

    free(table);
}

/*
 * Function: table_add
 * -------------------
 *  adds new entry to the table (assumes table don't have duplicates)
 *
 *  table: table to write to
 *  symbol: key of new entry
 *  address: value of new entry
 */
void table_add(table_t *table, const char *symbol, short address)
{
    int h = hash(symbol);
    table_node_t *node = node_new(symbol, address, table->data[h]);
    table->data[h] = node;
    table->size++;
}

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
bool table_contains(table_t *table, const char *symbol)
{
    int h = hash(symbol);

    for (table_node_t *p = table->data[h]; p; p = p->next) {
        if (!strcmp(p->key, symbol)) {
            return true;
        }
    }

    return false;
}

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
short table_get(table_t *table, const char *symbol)
{
    int h = hash(symbol);

    for (table_node_t *p = table->data[h]; p; p = p->next) {
        if (!strcmp(p->key, symbol)) {
            return p->val;
        }
    }

    return -1;
}
