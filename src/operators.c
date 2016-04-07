#include "operators.h"
#include <stdlib.h>
static operator_node *new_operator_node(char *data);
static void add_next(operator_node *current, char *data);
static operator_node* set_child(operator_node *current, char *data);

static operator_node *new_operator_node(char *data)
{
	operator_node *newNode = malloc(sizeof(*newNode));
	newNode->data = data;
	newNode->next = NULL;
	newNode->child = NULL;
	return newNode;
}

static void add_next(operator_node *current, char *data)
{
	operator_node *newNode = new_operator_node(data);
	newNode->next = current->next;
	current->next = newNode;
}

static operator_node* set_child(operator_node *current, char *data)
{
	operator_node *newNode = new_operator_node(data);
	current->child = newNode;
	return newNode;
}

operator_node* get_node()
{
	operator_node *root = new_operator_node("=");
	operator_node *current = set_child(root, "||");
	add_next(current, "|");
	current = set_child(current, "^^");
	add_next(current, "^");
	current = set_child(current, "&&");
	add_next(current, "&");
	current = set_child(current, "!=");
	add_next(current, "==");
	add_next(current, "<=");
	add_next(current, ">=");
	add_next(current, "<");
	add_next(current, ">");
	current = set_child(current, ">>");
	add_next(current, ">>>");
	add_next(current, "<<");
	current = set_child(current, "+");
	add_next(current, "-");
	add_next(current, "%");
	current = set_child(current, "*");
	add_next(current, "/");
	current = set_child(current, "**");
	current = set_child(current, ".");
	return root;
}
