#include "types.h"
#include "slice.h"
#include "util.h"
#include <stdbool.h>


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
    case VOID:
        return new_slice("void");
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
type get_index(type t) {
    //TODO: Generate an error if t is not an array
    return dereference(t);
}

