#ifndef TYPES_H_
#define TYPES_H_
#include "parser.h"
#include "util.h"
#include "slice.h"

DEFSTRUCT(primitive);
DEFSTRUCT(struct_type);
DEFSTRUCT(type);

typedef enum {SIGNED, UNSIGNED, FLOAT} numeric_type;

struct primitive {
    int bits;
    numeric_type type;
};
struct type {
    union {
        primitive numeric;
        struct_declaration *declared;
    } data;
    bool is_numeric;
};

type get_type(file_contents context, slice type_descriptor);
type make_numeric_type(numeric_type kind, int bits);
slice type_to_string(type t);
#endif
