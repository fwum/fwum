#ifndef LL_H_
#define LL_H_
#include "util.h"
#include <stdbool.h>

DEFSTRUCT(linked_list);
DEFSTRUCT(linked_node);
DEFSTRUCT(linked_iter);

struct linked_node {
	void *data;
	linked_node *next, *prev;
};
struct linked_list {
	linked_node *head, *tail;
};
struct linked_iter {
	linked_node *current;
};

linked_list ll_new();
void ll_add_first(linked_list *list, void *value);
void ll_add_last(linked_list *list, void *value);
void *ll_remove_last(linked_list *list);
void *ll_remove_first(linked_list *list);
bool ll_empty(linked_list *list);
void ll_clear(linked_list *list);

linked_iter ll_iter_head(linked_list *list);
linked_iter ll_iter_tail(linked_list *list);
void *ll_iter_next(linked_iter *iter);
void *ll_iter_prev(linked_iter *iter);
bool ll_iter_has_next(linked_iter *iter);
bool ll_iter_has_prev(linked_iter *iter);
void ll_iter_add_after(linked_iter *iter, void *data);
void ll_iter_add_before(linked_iter *iter, void *data);
void ll_iter_remove(linked_iter *iter);
#endif
