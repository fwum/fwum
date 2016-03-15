#include "semantic_analyzer.h"
#include "slice.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

static void semantic_error(char *error, char *file, int line);
static func_declaration *analyze_func(token_list *tokens);

file_contents analyze(token_list *tokens)
{
	file_contents contents = {NULL, NULL, NULL, NULL};
	parse_token *current = tokens->head;
	while(current != NULL)
	{
		if(equals(current->data, new_slice("struct")))
		{
			tokens->head = current->next;
			struct_declaration *dec = analyze_struct(tokens);
			current = tokens->head;
			if(contents.head == NULL)
				contents.head = dec;
			else if(contents.tail == NULL)
			{
				contents.head->next = dec;
				contents.tail = dec;
			}
			else
			{
				contents.tail->next = dec;
				contents.tail = dec;
			}
		}
		else
		{
			tokens->head = current;
			func_declaration *func = analyze_func(tokens);
			current = tokens->head;
			if(contents.funcHead == NULL)
				contents.funcHead = func;
			else if(contents.funcTail == NULL)
			{
				contents.funcHead->next = func;
				contents.funcTail = func;
			}
			else
			{
				contents.funcTail->next = func;
				contents.funcTail = func;
			}
		}
		if(current != NULL)
			current = current->next;
	}
	return contents;
}

struct_declaration *analyze_struct(token_list *tokens)
{
	parse_token *current = tokens->head;
	struct_declaration *dec = malloc(sizeof(*dec));
	dec->name = current->data;
	dec->head = dec->tail = NULL;
	dec->next = NULL;
	current = current->next;
	if(current->data.data[0] != '{')
		semantic_error("Expected opening brace after name in struct declaration.",
			current->origin.filename, current->origin.line);
	current = current->next;
	while(current->data.data[0] != '}')
	{
		struct_member *member = malloc(sizeof(*member));
		if(current->type != WORD)
			semantic_error("Struct members must be declared as <type> <value>;",
				current->origin.filename, current->origin.line);
		member->type = current->data;
		current = current->next;
		if(current->type != WORD)
			semantic_error("Struct members must be declared as <type> <value>;",
				current->origin.filename, current->origin.line);
		member->name = current->data;
		current = current->next;
		if(current->data.data[0] != ';')
			semantic_error("Struct members must be declared as <type> <value>;",
				current->origin.filename, current->origin.line);
		member->next = NULL;
		if(dec->head == NULL)
			dec->head = member;
		else if(dec->tail == NULL)
			dec->head->next = dec->tail = member;
		else
			dec->tail->next = member;
		current = current->next;
	}
	tokens->head = current;
	dec->next = NULL;
	return dec;
}

static func_declaration *analyze_func(token_list *tokens)
{
	parse_token *current = tokens->head;
	func_declaration *func = malloc(sizeof(*func));
	func->type = current->data;
	current = current->next;
	if(current == NULL || current->type != WORD)
		semantic_error("Function declaration must be in the form <rtype> <name>(<parameters>) {<block>}", current->origin.filename, current->origin.line);
	func->name = current->data;
	current = current->next;
	if(current == NULL || current->data.data[0] != '(')
		semantic_error("Function name must be followed by an open parenthesis", current->origin.filename, current->origin.line);
	current = current->next;
	statement *type = malloc(sizeof(*type));
	while(current->data.data[0] != ')')
	{
		if(current == NULL)
			semantic_error("Unexpected EOF encountered in function declaration", current->origin.filename, current->origin.line);
		type->type = TYPE;
		type->next = malloc(sizeof(*type));
		type = type->next;
		if(current->type != WORD)
			semantic_error("Parameter types must be a valid identifier", current->origin.filename, current->origin.line);
		type->data = current->data;
		statement *name = malloc(sizeof(*name));
		current = current->next;
		if(current == NULL)
			semantic_error("Unexpected EOF encountered in function declaration", current->origin.filename, current->origin.line);
		if(current->type != WORD)
			semantic_error("Parameter names must be valid identifiers.", current->origin.filename, current->origin.line);
		name->data = current->data;
		name->type = NAME;
		type->child = name;
		name->next = name->child = NULL;
		current = current->next;
	}
	func->params = type;
	current = current->next;
	if(current->data.data[0] != '{')
		semantic_error("Function bodies must start with an open brace ('{')", current->origin.filename, current->origin.line);
	current = current->next;
	int bracket_level = 0;
	while(bracket_level != 0 || current->data.data[0] != '}')
	{
		char value = current->data.data[0];
		if(value == '{')
			bracket_level += 1;
		if(value == '}')
			bracket_level -= 1;
		current = current->next;
	}
	current = current->next;
	tokens->head = current;
	return func;
}

void dump(file_contents contents)
{
	{
		struct_declaration *current = contents.head;
		while(current != NULL)
		{
			printf("STRUCT: %s\n", evaluate(current->name));
			struct_member *member = current->head;
			while(member != NULL)
			{
				printf("MEMBER: NAME: %s | TYPE: %s\n", evaluate(member->name), evaluate(member->type));
				member = member->next;
			}
			current = current->next;
		}
	}
	{
		func_declaration *current = contents.funcHead;
		while(current != NULL)
		{
			printf("FUNC: %s | TYPE : %s\n", evaluate(current->name), evaluate(current->type));
			current = current->next;
		}
	}

}

static void semantic_error(char *error, char *file, int line)
{
	fprintf(stderr, "Error encountered while analyzing %s at line %d:\n%s\n", file, line, error);
	exit(-1);
}
