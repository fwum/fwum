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
