#ifndef AST_H_
#define AST_H_
typedef enum ast_type
{
	ROOT, IMPORT, USING, VARTYPE, STRUCT, TYPE, FUNC, BIND, TYPED_BIND, ASSIGN
} ast_type;
typedef struct ast_node
{
	ast_type type;
	char* data;
	struct ast_node *next;
	struct ast_node *child;
} ast_node;
ast_node* new_node(ast_type type, char* data);
char* to_string(ast_node *root);
char* type_to_string(ast_type type);
int length_list(ast_node *start);
void add_child(ast_node *root, ast_node *child);
#endif
