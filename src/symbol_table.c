#include "symbol_table.h"

symbol_table *st_new() {
    symbol_table *table = new(table);
    table->symbols = hm_new();
    table->parent = NULL;
    return table;
}

symbol_table *st_sub_scope(symbol_table *parent) {
    symbol_table *table = st_new();
    table->parent = parent;
    return table;
}

bool st_contains(symbol_table *table, slice *name) {
    if(hm_has(table->symbols, slice_hash(*name), name)) {
        return true;
    } else if(table->parent != NULL) {
        return st_contains(table->parent, name);
    } else {
        return false;
    }
}

slice *st_get_type(symbol_table *table, slice *name) {
    slice *got = hm_get(table->symbols, slice_hash(*name), name);
    if(got != NULL) {
        return got;
    } else if(table->parent != NULL) {
        return st_get_type(table->parent, name);
    } else {
        return NULL;
    }
}

void st_put(symbol_table *table, slice *name, slice *type) {
    hm_put(table->symbols, slice_hash(*name), name, type);
}
