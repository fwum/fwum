#ifndef AST_H_
#define AST_H_
typedef enum ast_type
{
	ROOT, IMPORT, USING
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
int length_list(ast_node *start);
#endif
