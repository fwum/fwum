#ifndef PRINTING_H_
#define PRINTING_H_
#include "tokenizer.h"
#include "semantic_analyzer.h"
/*
Print the created AST to stdout, mostly for debugging purposes
*/
void dump(file_contents contents);
/*
Print an AST node to stdout (recursive)
*/
void dump_node(linked_list *list, int indentation);
/*
Print a list of tokens to stdout, mostly for debugging purposes
*/
void print_tlist(linked_list *list);
#endif
