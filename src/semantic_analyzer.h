#ifndef LEXICAL_ANALYZER_H_
#define LEXICAL_ANALYZER_H_
#include "util.h"
#include "tokenizer.h"
DEFSTRUCT(file_contents);
DEFSTRUCT(struct_declaration);
DEFSTRUCT(struct_member);
struct file_contents {
	struct_declaration *head, *tail;
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
void dump(file_contents contents);
file_contents analyze(token_list *tokens);
struct_declaration *analyze_struct(token_list *token);
#endif
