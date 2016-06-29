#ifndef SYMBOL_TABLE_H_
#define SYMBOL_TABLE_H_
#include "util.h"
#include "hashmap.h"
#include "stdbool.h"
#include "slice.h"
#include "types.h"

DEFSTRUCT(symbol_table);
DEFSTRUCT(symbol_entry);

struct symbol_table {
    hash_map *symbols;
    symbol_table *parent;
};

symbol_table *st_new();
symbol_table *st_sub_scope(symbol_table *parent);
type get_type(symbol_table *context, slice type_descriptor);
bool st_contains(symbol_table *table, slice *name);
type *st_get_type(symbol_table *table, slice *name);
void st_put(symbol_table *table, slice *name, type *type);
#endif
