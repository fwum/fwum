#include "operators.h"
#include "semantic_analyzer.h"
#include "util.h"
#include <stdlib.h>
static operator_node *new_operator_node(char *data, statement_type type);
static void add_next(operator_node *current, char *data, statement_type type);
static operator_node* set_child(operator_node *current, char *data, statement_type type);

static operator_node *new_operator_node(char *data, statement_type type)
{
	operator_node *newNode = new(newNode);
	newNode->data = data;
	newNode->next = NULL;
	newNode->child = NULL;
	newNode->operatorType = type;
	return newNode;
}

static void add_next(operator_node *current, char *data, statement_type type)
{
	operator_node *newNode = new_operator_node(data, type);
	newNode->next = current->next;
	current->next = newNode;
}

static operator_node* set_child(operator_node *current, char *data, statement_type type)
{
	operator_node *newNode = new_operator_node(data, type);
	current->child = newNode;
	return newNode;
}

operator_node* get_node()
{
	operator_node *root = new_operator_node("=", OP_ASSGIN);
	operator_node *current = set_child(root, "||", OP_BOOL_OR);
	add_next(current, "|", OP_BIT_OR);
	current = set_child(current, "^^", OP_BOOL_XOR);
	add_next(current, "^", OP_BIT_XOR);
	current = set_child(current, "&&", OP_BOOL_AND);
	add_next(current, "&", OP_BIT_AND);
	current = set_child(current, "!=", OP_NOT_EQUAL);
	add_next(current, "==", OP_EQUAL);
	add_next(current, "<=", OP_LESS_EQUAL);
	add_next(current, ">=", OP_GREATER_EQUAL);
	add_next(current, "<", OP_LESS);
	add_next(current, ">", OP_GREATER);
	current = set_child(current, ">>", OP_SHIFT_RIGHT);
	add_next(current, ">>>", OP_BIT_SHIFT_RIGHT);
	add_next(current, "<<", OP_SHIFT_LEFT);
	current = set_child(current, "+", OP_ADD);
	add_next(current, "-", OP_SUB);
	add_next(current, "%", OP_MOD);
	current = set_child(current, "*", OP_MULT);
	add_next(current, "/", OP_DIV);
	current = set_child(current, "**", OP_EXP);
	current = set_child(current, ".", OP_MEMBER);
	return root;
}
