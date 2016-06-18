#ifndef OPERATOR_H_
#define OPERATOR_H_
#include "parser.h"
#include "linked_list.h"
#include "util.h"

DEFSTRUCT(operator_node);

struct operator_node {
	char *data;
	statement_type operatorType;
};

linked_list *get_node();
bool is_operator(slice op);
bool is_unary_operator(slice op);
#endif
