#include "symbol_table.h"
#include "slice.h"

symbol_table *st_new() {
    symbol_table *table = new(table);
    table->symbols = hm_new_eqfunc(&slice_eq_voidptr);
    table->parent = NULL;
    return table;
}

symbol_table *st_sub_scope(symbol_table *parent) {
    symbol_table *table = st_new();
    table->parent = parent;
    return table;
}

type get_type(symbol_table *context, slice type_descriptor) {
    char first = type_descriptor.data[0];
    slice amt = make_slice(type_descriptor.data + 1, type_descriptor.len - 1);
    type t;
    if(first == '$') {
        t.kind = WRAPPED;
        t.data.wrapper.isPtr = true;
        t.data.wrapper.typeOf = new(t.data.wrapper.typeOf);
        *t.data.wrapper.typeOf = get_type(context, make_slice(type_descriptor.data + 1, type_descriptor.len - 1));
    } else if((first == 'u' || first == 'i' || first == 'f') && is_numeric(amt)) {
        t.kind = PRIMITIVE;
        optional op = parse_int(amt);
		int size = *((int*)op_get(op));
        t.data.numeric.bits = size;
        switch(first) {
        case 'u':
            t.data.numeric.type = UNSIGNED;
            break;
        case 'i':
            t.data.numeric.type = SIGNED;
            break;
        case 'f':
            t.data.numeric.type = FLOAT;
            break;
        }
    } else if(equals_string(type_descriptor, "void")) {
        t.kind = VOID;
    } else {
        t = *st_get_type(context, &type_descriptor);
    }
    return t;
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

type *st_get_type(symbol_table *table, slice *name) {
    type *got = hm_get(table->symbols, slice_hash(*name), name);
    if(got != NULL) {
        return got;
    } else if(table->parent != NULL) {
        return st_get_type(table->parent, name);
    } else {
        return NULL;
    }
}

void st_put(symbol_table *table, slice *name, type *type) {
    hm_put(table->symbols, slice_hash(*name), name, type);
}
