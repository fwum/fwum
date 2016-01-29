#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include "symbols.h"

symbol* new_entry(char *name, char *type)
{
	symbol *s = malloc(sizeof(*s));
	s->name = name;
	s->type = type;
	s->next = NULL;
}

char* get_type(symbol *start, char *name)
{
	if(start == NULL)
	{
		fprintf(stderr, "get_type did not encounter the given symbol\n");
		exit(-1);
	}
	if(strcomp(start->name, name) == 0)
		return start->type;
	else
		return get_type(start->next, name);
}

bool contains(symbol *start, char *name)
{
	if(start == NULL)
		return false;
	else if(strcomp(start->name, name) == 0)
		return true;
	else
		return contains(start->next, name);
}

type* new_type(char *name, symbol *members)
{
	type *t = malloc(sizeof(*t));
	t->name = name;
	t->members = members;
	return t;
}

bool has_member(type *t, char *name)
{
	return contains(t->members, name);
}

bool has_member_oftype(type *t, char *name, char *type)
{
	return strcomp(get_type(t->members, name), type) == 0;
}
