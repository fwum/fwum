#include "operators.h"
#include "parser.h"
#include "util.h"
#include "linked_list.h"
#include <stdlib.h>
#include <stdarg.h>

static operator_node *new_operator_node(char *data, statement_type type);
static void add_level(linked_list *list, int number_ops, ...);

static operator_node *new_operator_node(char *data, statement_type type) {
	operator_node *newNode = new(newNode);
	newNode->data = data;
	newNode->operatorType = type;
	return newNode;
}

linked_list* get_node() {
	linked_list *list = ll_new();
	add_level(list, 4, "=", OP_ASSIGN, ":=", OP_INIT);
	add_level(list, 4, "||", OP_BOOL_OR, "|", OP_BIT_OR);
	add_level(list, 4, "^^", OP_BOOL_XOR, "^", OP_BIT_XOR);
	add_level(list, 4, "&&", OP_BOOL_AND, "&", OP_BIT_AND);
	add_level(list, 12, "!=", OP_NOT_EQUAL, "==", OP_EQUAL, "<=", OP_LESS_EQUAL, ">=", OP_GREATER_EQUAL, "<", OP_LESS, ">", OP_GREATER);
	add_level(list, 6, ">>", OP_SHIFT_RIGHT, ">>>", OP_BIT_SHIFT_RIGHT, "<<", OP_SHIFT_LEFT);
	add_level(list, 6, "+", OP_ADD, "-", OP_SUB, "%", OP_MOD);
	add_level(list, 4, "*", OP_MULT, "/", OP_DIV);
	add_level(list, 2, "**", OP_EXP);
	add_level(list, 2, ".", OP_MEMBER);
	add_level(list, 4, "$", OP_DEREF, "#", OP_GETREF);
	return list;
}

static void add_level(linked_list *list, int number_args, ...) {
	linked_list *current = ll_new();
	va_list args;
	va_start(args, number_args);
	for(int i = 0; i < number_args / 2; i++) {
		char *operator = va_arg(args, char*);
		statement_type type = va_arg(args, statement_type);
		ll_add_last(current, new_operator_node(operator, type));
	}
	va_end(args);
	ll_add_last(list, current);
}

bool is_operator(slice op) {
	linked_list *outer = get_node();
	linked_iter iterator = ll_iter_head(outer);
	while(ll_iter_has_next(&iterator)) {
		linked_list *inner = ll_iter_next(&iterator);
		linked_iter inner_iterator = ll_iter_head(inner);
		while(ll_iter_has_next(&inner_iterator)) {
			operator_node *current = ll_iter_next(&inner_iterator);
			if(equals_string(op, current->data)) {
				free(current);
				return true;
			}
		}
		free(inner);
	}
	free(outer);
	return false;
}
bool is_unary_operator(slice op) {
	return equals_string(op, "$") || equals_string(op, "#");
}
