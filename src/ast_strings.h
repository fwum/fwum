#ifndef AST_STRINGS_H_
#define AST_STRINGS_H_
#include "slice.h"
#include "semantic_analyzer.h"
statement_type statement_from_slice(slice string);
char *statement_to_string(statement_type type);
#endif
