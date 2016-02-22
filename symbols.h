#ifndef SYMBOLS_H_
#define SYMBOLS_H_
#include "ast.h"
struct func_entry;
struct type_entry;
struct var_entry;
struct namespace_entry;
typedef struct func_entry func_entry;
typedef struct type_entry type_entry;
typedef struct var_entry var_entry;
typedef struct namespace_entry namespace_entry;
struct func_entry {
	namespace_entry *namespace;
	char *name;
	char *returnType;
	char **params;
	int num_params;
	func_entry *next;
};
struct type_entry {
	namespace_entry *namespace;
	char *name;
	enum {STRUCTURE, PRIMITIVE, FUNCTION} typeOf;
	union {
		func_entry *function;
		type_entry *members;
		struct {
			enum {SIGNED, UNSIGNED, FLOAT} type;
			int bits;
		} primitive;
	} data;
	type_entry *next;
};
struct var_entry {
	namespace_entry *namespace;
	char *name;
	type_entry type;
	var_entry *next;
};
struct namespace_entry {
	char *name;
	namespace_entry *next;
};
type_entry *get_type_table(ast_node *root);
func_entry *get_func_table(ast_node *root);
var_entry *get_all_vars(ast_node *block);
namespace_entry *get_namespace_table(ast_node *root);
void concat_type_table(type_entry *table1, type_entry *table2);
void concat_func_table(func_entry *table1, func_entry *table2);
void concat_var_table(var_entry *table1, var_entry *table2);
void concat_name_table(namespace_entry *table1, namespace_entry *table2);
void verify(ast_node *root, type_entry *typeTable, func_entry *funcTable, namespace_entry *namespace);
#endif
