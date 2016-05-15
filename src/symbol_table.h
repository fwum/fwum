#ifndef SYMBOL_TABLE_H_
#define SYMBOL_TABLE_H_
#include "util.h"
#include "hashmap.h"
#include "stdbool.h"
#include "slice.h"

DEFSTRUCT(symbol_table);
DEFSTRUCT(symbol_entry);

struct symbol_table {
    hash_map *symbols;
    symbol_table *parent;
};

symbol_table *st_new();
symbol_table *st_sub_scope(symbol_table *parent);
bool st_contains(symbol_table *table, slice *name);
slice *st_get_type(symbol_table *table, slice *name);
void st_put(symbol_table *table, slice *name, slice *type);
#endif
