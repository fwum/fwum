#ifndef LEXICAL_ANALYZER_H_
#define LEXICAL_ANALYZER_H_
#include "util.h"
#include "tokenizer.h"
DEFSTRUCT(file_contents);
DEFSTRUCT(struct_declaration);
DEFSTRUCT(struct_member);
DEFSTRUCT(func_declaration);
DEFSTRUCT(statement);
DEFSTRUCT(parameter);
struct file_contents {
	struct_declaration *head, *tail;
	func_declaration *funcHead, *funcTail;
};
struct struct_declaration {
	slice name;
	struct_member *head, *tail;
	source_origin origin;
	struct_declaration *next;
};
struct struct_member {
	slice type, name;
	struct_member *next;
};
struct func_declaration {
	slice type, name;
	statement *params;
	statement *root;
	func_declaration *next;
};
typedef enum statement_type {OPERATOR, IF, WHILE, BLOCK, TYPE, NAME} statement_type;
struct statement {
	slice data;
	statement_type type;
	statement *next, *child;
};

void dump(file_contents contents);
file_contents analyze(token_list *tokens);
struct_declaration *analyze_struct(token_list *token);
#endif
