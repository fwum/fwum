#include "types.h"
#include "slice.h"
#include "util.h"

type get_type(file_contents context, slice type_descriptor) {
    char first = type_descriptor.data[0];
    slice amt = make_slice(type_descriptor.data + 1, type_descriptor.len - 1);
    type t;
    if((first == 'u' || first == 'i' || first == 'f') && is_numeric(amt)) {
        t.is_numeric = true;
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
    } else {
        t.is_numeric = false;
        t.data.declared = NULL;
        linked_list *structs = context.structs;
        linked_iter iterator = ll_iter_head(structs);
        while(ll_iter_has_next(&iterator)) {
            struct_declaration *dec = ll_iter_next(&iterator);
            if(equals(type_descriptor, dec->name))
                t.data.declared = dec;
        }
    }
    return t;
}

type make_numeric_type(numeric_type kind, int bits) {
    type t;
    t.is_numeric = true;
    t.data.numeric.bits = bits;
    t.data.numeric.type = kind;
    return t;
}

slice type_to_string(type t) {
    if(t.is_numeric) {
        char *str = string(3);
        primitive num = t.data.numeric;
        switch(num.type) {
        case UNSIGNED:
            str[0] = 'u';
            break;
        case SIGNED:
            str[0] = 'i';
            break;
        case FLOAT:
            str[0] = 'f';
            break;
        }
        str[1] = num.bits / 10;
        str[2] = num.bits % 10;
        return new_slice(str);
    } else {
        return t.data.declared->name;
    }
}

bool type_equals(type t1, type t2) {
    if(t1.is_numeric != t2.is_numeric)
        return false;
    if(t1.is_numeric) {
        return t1.data.numeric.bits == t2.data.numeric.bits && t1.data.numeric.type == t2.data.numeric.type;
    } else {
        return t1.data.declared == t2.data.declared;
    }
}
