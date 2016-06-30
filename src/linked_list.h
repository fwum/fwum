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
	linked_list *origin;
	linked_node *current;
	bool goesForward;
};

linked_list *ll_new();
linked_list *ll_duplicate(linked_list *list);
void ll_add_first(linked_list *list, void *value);
void ll_add_last(linked_list *list, void *value);
void ll_concat(linked_list *mutated, linked_list *newItems);
void *ll_get_first(linked_list *list);
void *ll_get_last(linked_list *list);
void *ll_get(linked_list *list, int index);
void *ll_remove_last(linked_list *list);
void *ll_remove_first(linked_list *list);
bool ll_empty(linked_list *list);
void ll_clear(linked_list *list);
void ll_destroy(linked_list *list);
void ll_delete_all(linked_list *list);
int ll_size(linked_list *list);

linked_iter ll_iter_head(linked_list *list);
linked_iter ll_iter_tail(linked_list *list);
void *ll_iter_next(linked_iter *iter);
bool ll_iter_has_next(linked_iter *iter);
void ll_iter_clear_to_current(linked_iter *iter);
void ll_iter_clear_remaining(linked_iter *iter);
#endif
