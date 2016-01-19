#include "ast.h"
#include <stdlib.h>
#include <string.h>

ast_node* new_node(ast_type type, char* data)
{
	ast_node *new_node = malloc(sizeof(*new_node));
	new_node->type = type;
	new_node->data = data;
	new_node->next = NULL;
	new_node->child = NULL;
	return new_node;
}

int length_list(ast_node *first)
{
	int len = 0;
	while(first->next != NULL)
	{
		first = first->next;
		len++;
	}
	return len;
}

char* to_string(ast_node *root)
{
	char *child = "", *next = "", *type;
	switch(root->type) {
	case ROOT:
		type = "ROOT ";
		break;
	case IMPORT:
		type = "IMPORT ";
		break;
	case USING:
		type = "USING ";
		break;
	}
	if(root->child != NULL)
		child = to_string(root->child);
	if (root->next != NULL)
		next = to_string(root->next);
	int result_len = strlen(root->data) + 2;
	result_len += strlen(child);
	result_len += strlen(next);
	result_len += strlen(type);
	char* result = malloc(sizeof(*result) * (result_len + 1));
	result[result_len] = '\0';
	strcat(result, "(");
	strcat(result, type);
	strcat(result, root->data);
	strcat(result, child);
	strcat(result, ")");
	strcat(result, next);
	return result;
}
