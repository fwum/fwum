#ifndef OPERATOR_H_
#define OPERATOR_H_
struct operator_node;
typedef struct operator_node operator_node;

struct operator_node {
	char *data;
	operator_node *next;
	operator_node *child;
};

operator_node* get_node();
#endif
