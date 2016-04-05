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
	statement *paramHead, *paramTail;
	statement *root;
	func_declaration *next;
};
typedef enum statement_type {OP_ADD, OP_SUB, OP_MULT, OP_DIV, OP_MOD, OP_BIT_AND,
	OP_BOOL_AND, OP_BIT_OR, OP_BOOL_OR, OP_BIT_XOR, OP_BOOL_XOR, OP_BIT_NOT, OP_BOOL_NOT,
	OP_GREATER, OP_LESS, OP_GREATER_EQUAL, OP_LESS_EQUAL, OP_EQUAL, OP_NOT_EQUAL,
	OP_ASSIGN, OP_MEMBER, OP_GETREF, OP_DEREF, OP_INDEX,
	RETURN, BREAK, CONTINUE,
	FUNC_CALL,
	STACK_INIT, HEAP_INIT,
	IF, WHILE, FOR, FOREACH,
	BLOCK, TYPE, NAME,
	ROOT,
	STRING, CHAR, NUM,
	VALUE_TRUE, VALUE_FALSE} statement_type;
struct statement {
	slice data;
	statement_type type;
	statement *next, *child, *parent;
};
file_contents analyze(token_list *tokens);
struct_declaration *analyze_struct(token_list *token);
#endif
