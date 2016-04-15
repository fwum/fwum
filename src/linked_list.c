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
