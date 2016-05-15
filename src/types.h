#ifndef TYPES_H_
#define TYPES_H_
#include "parser.h"
#include "util.h"
#include "slice.h"

DEFSTRUCT(primitive);
DEFSTRUCT(struct_type);
DEFSTRUCT(type);

struct primitive {
    int bits;
    enum {SIGNED, UNSIGNED, FLOAT} type;
};
struct type {
    union {
        primitive numeric;
        struct_declaration *declared;
    } data;
    bool is_numeric;
};

type get_type(file_contents context, slice type_descriptor);
#endif
