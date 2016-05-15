#include "types.h"
#include "slice.h"

type get_type(file_contents context, slice type_descriptor) {
    char first = type_descriptor.data[0];
    slice amt = make_slice(type_descriptor.data + 1, type_descriptor.len - 1);
    type t;
    if((first == 'u' || first == 'i' || first == 'f') && is_numeric(amt)) {
        t.is_numeric = true;
        int size = parse_int(amt);
        t.primitive.bits = size;
        switch(first) {
        case 'u':
            t.primitive.type = UNSIGNED;
            break;
        case 'i':
            t.primitive.type = SIGNED;
            break;
        case 'f':
            t.primitive.type = FLOAT;
            break;
        }
    } else {
        t.is_numeric = false;
        t.declared = NULL;
        linked_list *structs = context.structs;
        linked_iter iterator = ll_iter_head(structs);
        while(ll_iter_has_next(&iterator)) {
            struct_declaration *dec = ll_iter_next(&iterator);
            if(equals(type_descriptor, dec->name))
                t.declared = dec;
        }
    }
    return t;
}
