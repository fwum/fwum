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
	int list = length_list(root);
	char** same_level = NULL;
	if(list != 0)
		same_level = malloc(sizeof(*same_level) * list);
	char* child = "";
	if(root->child != NULL)
		child = to_string(root->child);
	int result_len = strlen(root->data) + 2;
	result_len += strlen(child);
	for(int i = 0; i < list; i++)
		result_len += strlen(same_level[i]);
	char* result = malloc(sizeof(*result) * (result_len + 1));
	result[result_len] = '\0';
	strcat(result, "(");
	strcat(result, root->data);
	strcat(result, child);
	strcat(result, ")");
	for(int i = 0; i < list; i++)
		strcat(result, same_level[i]);
	return result;
}
