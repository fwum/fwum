#ifndef LL_H_
#define LL_H_
#include "util.h"
DEFSTRUCT(linked_list);
DEFSTRUCT(linked_node);
struct linked_node {
	void *data;
	linked_node *next, *prev;
};
struct linked_list {
	linked_node *head, *tail;
};
void ll_add_first(linked_list *list, void *value);
void ll_add_last(linked_list *list, void *value);
void *ll_remove_last(linked_list *list);
void *ll_remove_first(linked_list *list);
#endif
