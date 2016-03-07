#ifndef LEXICAL_ANALYZER_H_
#define LEXICAL_ANALYZER_H_
#include "util.h"
DEFSTRUCT(namespace);
DEFSTRUCT(struct_definition);
DEFSTRUCT(var_declaration);
DEFSTRUCT(var_list);
DEFSTRUCT(func_definition);
DEFSTRUCT(operation);
DEFSTRUCT(call);
DEFSTRUCT(statement);
DEFSTRUCT(statement_list);
struct namespace {
	namespace *parent;
	char *name;
};
struct struct_definition {
	namespace *ns;
	char *name;
	var_list members;
};
struct var_list {
	var_declaration *head, *tail;
};
struct var_declaration {
	namespace *ns;
	char *type;
	char *name;
	var_declaration *next;
};
struct func_definition {
	namespace *ns;
	char *name;
	char *retunType;
	var_list members;
};
struct operation {
	char *operator;
	statment_list operands;
};
struct call {
	char *funcName;
	statement_list parameters;
};
struct statement {
	union {var_declaration *varDec, call call, char *name, operation op} value;
	enum {VAR_DELARATION, CALL, VARIABLE, OPERATION} type;
};
struct statement_list {
	statement *head, *tail;
};
//TODO: Define functions
#endif
