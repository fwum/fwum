#ifndef SYMBOLS_H_
#define SYMBOLS_H_
struct func_entry;
struct type_entry;
struct var_entry;
struct func_table;
struct type_table;
struct var_table;
typedef struct func_entry func_entry;
typedef struct type_entry type_entry;
typedef struct var_entry var_entry;
struct func_entry {
	char *name;
	char *returnType;
	char **params;
	int num_params;
	func_entry *next;
};
struct type_entry {
	char *name;
	enum {STRUCT, PRIMITIVE, FUNCTION};
	union {
		func_entry function;
		type_table members;
		struct {
			enum {SIGNED, UNSIGNED, FLOAT};
			int bits;
		} primitive;
	};
	type_entry *next;
};
struct var_entry {
	char *name;
	type_entry type;
	var_entry *next;
};
#endif
