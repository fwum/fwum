#ifndef AST_STRINGS_H_
#define AST_STRINGS_H_
#include "slice.h"
#include "semantic_analyzer.h"
//Parse a statement type from a string
statement_type statement_from_slice(slice string);
//Convert a statement type to a string
char *statement_to_string(statement_type type);
#endif
