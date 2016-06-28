#include "types.h"
#include "slice.h"
#include "util.h"
#include <stdbool.h>

type get_type(file_contents context, slice type_descriptor) {
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
    } else {
        t.kind = STRUCT;
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
    t.kind = PRIMITIVE;
    t.data.numeric.bits = bits;
    t.data.numeric.type = kind;
    return t;
}

slice type_to_string(type t) {
    switch(t.kind) {
    case PRIMITIVE:
    //TODO: handle numbers with greater >99 bits
    {
        primitive num = t.data.numeric;
        int digits = 0;
        int temp = num.bits;
        while(temp > 0) {
            temp /= 10;
            digits ++;
        }
        char *str = string(digits + 1);
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
        temp = num.bits;
        for(int i = 0; i < digits; i++) {
            str[digits - i] = '0' + num.bits % 10;
            num.bits /= 10;
        }
        return new_slice(str);
    }
    case WRAPPED: {
        slice innerString = type_to_string(*(t.data.wrapper.typeOf));
        char *newString = string(innerString.len + 1);
        for(int i = 0; i < innerString.len; i++)
            newString[i] = innerString.data[i];
        newString[innerString.len] = '*';
        return new_slice(newString);
    }
    case STRUCT:
        return t.data.declared->name;
    default:
        return new_slice("");
    }
}

bool type_equals(type t1, type t2) {
    if(t1.kind != t2.kind)
        return false;
    switch(t1.kind) {
    case PRIMITIVE:
        return t1.data.numeric.bits == t2.data.numeric.bits && t1.data.numeric.type == t2.data.numeric.type;
    case WRAPPED:
        return type_equals(*(t1.data.wrapper.typeOf), *(t2.data.wrapper.typeOf));
    case STRUCT:
        return t1.data.declared == t2.data.declared;
    default:
        return false;
    }
}

type reference(type t) {
    type ptr;
    ptr.kind = WRAPPED;
    type *boxed = new(boxed);
    *boxed = t;
    ptr.data.wrapper.typeOf = boxed;
    ptr.data.wrapper.isPtr = true;
    return ptr;
}

type dereference(type t) {
    //TODO: Generate an error if t is not a pointer
    return *(t.data.wrapper.typeOf);
}
type index(type t) {
    //TODO: Generate an error if t is not an array
    return dereference(t);
}
