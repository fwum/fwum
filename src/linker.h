#ifndef LINKER_H_
#define LINKER_H_
typedef struct searchpath {
	char *path;
	struct searchpath *next;
} searchpath;
ast_node *resolve(char *name, searchpath *path);
#endif
