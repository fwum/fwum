#ifndef SYMBOLS_H_
#define SYMBOLS_H_
#include <stdbool.h>
typedef struct symbol {
	char *name, *type;
	struct symbol *next;
} symbol;
symbol* new_entry(char *name, char *type);
char* get_type(symbol *start, char *name);
bool contains(symbol *start, char *name);
#endif
