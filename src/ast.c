#include "ast.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "slice.h"

ast_node* new_node(ast_type type, char* data)
{
	ast_node *new_node = malloc(sizeof(*new_node));
	new_node->type = type;
	new_node->data = data;
	new_node->next = NULL;
	new_node->child = NULL;
	return new_node;
}

void add_child(ast_node *root, ast_node *child)
{
	if(root->child == NULL)
		root->child = child;
	else
	{
		ast_node *current = root->child;
		while(current->next != NULL)
			current = current->next;
		current->next = child;
	}
}

char* type_to_string(ast_type type)
{
	switch(type) {
	case ROOT:
		return "ROOT ";
	case IMPORT:
		return "IMPORT ";
	case USING:
		return "USING ";
	case CIMPORT:
		return "CIMPORT ";
	case VARTYPE:
		return "VARTYPE ";
	case STRUCT:
		return "STRUCT ";
	case TYPE:
		return "TYPE ";
	case FUNC:
		return "FUNC ";
	case BIND:
		return "BIND ";
	case TYPED_BIND:
		return "TYPED_BIND ";
	case ASSIGN:
		return "ASSIGN ";
	case STRING:
		return "STRING ";
	case NUMBER:
		return "NUMBER ";
	case VAR:
		return "VAR ";
	case CALL:
		return "CALL ";
	case BLOCK:
		return "BLOCK ";
	case OPERATOR:
		return "OPERATOR ";
	case CONTROL:
		return "CONTROL ";
	case RETURN:
		return "RETURN ";
	}
	fprintf(stderr, "type_to_string was passed an unexpected value.");
	exit(-1);
}

char* to_string(ast_node *root)
{
	char *child = "", *next = "", *type;
	type = type_to_string(root->type);
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

bool is_number_literal(slice literal)
{
	for(int i = literal.begin; i < literal.end; i++)
		if(!(literal.data[i] >= '0' && literal.data[i] <= '9') && !(literal.data[i] == '.' && i != literal.begin))
			return false;
	return true;
}

bool is_string_literal(slice literal)
{
	if(get(literal, 0) != '"' || get(literal, literal.end - literal.begin - 1) != '"')
		return false;
	for(int i = literal.begin + 1; i < literal.end - 1; i++)
		if(literal.data[i] == '"')
			return false;
	return true;
}

bool is_identifier_literal(slice literal)
{
	for(int i = literal.begin; i < literal.end; i++)
		if(!is_identifier(literal.data[i]))
			return false;
	return true;
}

void free_tree(ast_node *root)
{
	ast_node *current = root->child;
	while(current != NULL)
	{
		free_tree(current);
		current = current->next;
	}
	free(root);
}
