#ifndef OPERATOR_H_
#define OPERATOR_H_
#include "semantic_analyzer.h"

struct operator_node;
typedef struct operator_node operator_node;

struct operator_node {
	char *data;
	operator_node *next;
	operator_node *child;
	statement_type operatorType;
};

operator_node* get_node();
#endif
