#include "linked_list.h"
#include "util.h"
#include "stdlib.h"

void ll_add_first(linked_list *list, void *value)
{
	linked_node *node = new(node);
	node->data = value;
	node->next = list->head;
	node->prev = NULL;
	list->head = node;
	if(list->tail == NULL)
		list->tail = node;
}

void ll_add_last(linked_list *list, void *value)
{
	if(list->head == NULL)
	{
		ll_add_first(list, value);
	}
	else
	{
		linked_node *node = new(node);
		node->data = value;
		node->prev = list->tail;
		node->next = NULL;
		list->tail = node;
	}
}

void *ll_remove_last(linked_list *list)
{
	void *data = list->tail->data;
	if(list->head == list->tail)
	{
		free(list->head);
		list->head = list->tail = NULL;
	}
	else
	{
		linked_node *tail = list->tail;
		list->tail = list->tail->prev;
		list->tail->next = NULL;
		free(tail);
	}
	return data;
}

void *ll_remove_first(linked_list *list)
{
	void *data = list->head->data;
	if(list->head == list->tail)
	{
		free(list->head);
		list->head = list->tail = NULL;
	}
	else
	{
		linked_node *head = list->head;
		list->head = list->head->next;
		list->head->next = NULL;
		free(head);
	}
	return data;
}

linked_iter ll_iter_head(linked_list *list)
{
	linked_iter iterator;
	iterator.current = list->head;
	return iterator;
}

linked_iter ll_iter_tail(linked_list *list)
{
	linked_iter iterator;
	iterator.current = list->tail;
	return iterator;
}

void *ll_iter_next(linked_iter *iter)
{
	if(iter->current == NULL)
	{
		return NULL;
	}
	else
	{
		void *value = iter->current->data;
		iter->current = iter->current->next;
		return value;
	}
}

void *ll_iter_prev(linked_iter *iter);

bool ll_iter_has_next(linked_iter *iter)
{
	return iter->current != NULL;
}

bool ll_empty(linked_list *list)
{
	return list->head == NULL;
}

void ll_clear(linked_list *list)
{
	list->tail = NULL;
	while(list->head != NULL)
	{
		linked_node *next = list->head->next;
		free(list->head);
		list->head = next;
	}
}

bool ll_iter_has_prev(linked_iter *iter);
void ll_iter_add_after(linked_iter *iter, void *data);
void ll_iter_add_before(linked_iter *iter, void *data);
void ll_iter_remove(linked_iter *iter);
