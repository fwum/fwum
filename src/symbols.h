#ifndef SYMBOLS_H_
#define SYMBOLS_H_
#include <stdbool.h>
typedef struct symbol {
	char *name, *type;
	struct symbol *next;
} symbol;
typedef struct type {
	char *name;
	symbol *members;
	struct type *next;
} type;
symbol* new_entry(char *name, char *type);
char* get_type(symbol *start, char *name);
bool contains(symbol *start, char *name);
type* new_type(char *name, symbol *members);
bool has_member(type *t, char *name);
bool has_member_oftype(type *t, char *name, char *type);
#endif
