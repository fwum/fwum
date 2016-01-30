#ifndef CBACKEND_H_
#define CBACKEND_H_
#include "ast.h"
#include <stdio.h>
void compile(ast_node *root, FILE* stream);
#endif
