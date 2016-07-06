#ifndef TYPES_H_
#define TYPES_H_
#include "parser.h"
#include "util.h"
#include "slice.h"
#include <stdbool.h>
DEFSTRUCT(primitive);
DEFSTRUCT(struct_type);
DEFSTRUCT(wrapped_type);
DEFSTRUCT(type);

typedef enum {SIGNED, UNSIGNED, FLOAT} numeric_type;
typedef enum {PRIMITIVE, STRUCT, ENUM, WRAPPED, VOID} data_type;

struct wrapped_type {
	type *typeOf;
	bool isPtr;
};
struct primitive {
    int bits;
    numeric_type type;
};
struct type {
    union {
        primitive numeric;
        struct_declaration *declared;
        enum_declaration *enum_dec;
		wrapped_type wrapper;
    } data;
    data_type kind;
};

type make_numeric_type(numeric_type kind, int bits);
type reference(type t);
type dereference(type t);
type get_index(type t);
bool type_equals(type t1, type t2);
slice type_to_string(type t);
#endif
