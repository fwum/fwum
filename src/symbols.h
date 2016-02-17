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
typedef struct func_table func_table;
typedef struct type_table type_table;
typedef struct var_table var_table;
struct func_entry {
	char *name;
	char *returnType;
	char **params;
	int num_params;
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
};
struct var_entry {
	char *name;
	type_entry type;
};
struct func_table {
	func_entry entry;
	func_table *next;	
};
#endif
