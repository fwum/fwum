#ifndef AST_H_
#define AST_H_
#include <stdbool.h>
#include "slice.h"
typedef enum ast_type
{
	ROOT, IMPORT, USING, CIMPORT, VARTYPE, STRUCT, TYPE, FUNC, BIND, TYPED_BIND, ASSIGN,
	STRING, NUMBER, VAR, CALL, BLOCK, OPERATOR, CONTROL, RETURN
} ast_type;
typedef struct ast_node
{
	ast_type type;
	int sourceLine;
	char *sourceFile;
	char* data;
	struct ast_node *next;
	struct ast_node *child;
} ast_node;
ast_node* new_node(ast_type type, char* data, int sourceLine, char *sourceFileName);
char* to_string(ast_node *root);
char* type_to_string(ast_type type);
void add_child(ast_node *root, ast_node *child);
bool is_number_literal(slice literal);
bool is_string_literal(slice literal);
bool is_identifier_literal(slice literal);
void free_tree(ast_node *root);
#endif
