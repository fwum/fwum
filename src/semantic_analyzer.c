#include "semantic_analyzer.h"
#include "slice.h"
#include "util.h"
#include "printing.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

static void semantic_error(char *error, source_origin origin);
static func_declaration *analyze_func(token_list *tokens);
static statement *get_expression(token_list *tokens);

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
		else if(equals(current->data, new_slice("func")))
		{
			tokens->head = current->next;
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
	struct_declaration *dec = new(dec);
	dec->name = current->data;
	dec->head = dec->tail = NULL;
	dec->next = NULL;
	current = current->next;
	if(current->data.data[0] != '{')
		semantic_error("Expected opening brace after name in struct declaration.", current->origin);
	current = current->next;
	while(current->data.data[0] != '}')
	{
		struct_member *member = new(member);
		if(current->type != WORD)
			semantic_error("Struct members must be declared as <value> : <type>;", current->origin);
		member->name = current->data;
		current = current->next;
		if(current->type != SYMBOL || current->data.data[0] != ':' || (current = current->next)->type != WORD)
			semantic_error("Struct members must be declared as <value> : <type>;",current->origin);
		member->type = current->data;
		current = current->next;
		if(current->data.data[0] != ';')
			semantic_error("Struct members must be declared as <value> : <type>;", current->origin);
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
	func_declaration *func = new(func);
	if(current == NULL || current->type != WORD)
		semantic_error("Function declaration must be in the form func <name>(<parameters>) : <returntype> {<block>}", current->origin);
	func->name = current->data;
	current = current->next;
	if(current == NULL || current->data.data[0] != '(')
		semantic_error("Function name must be followed by an open parenthesis", current->origin);
	current = current->next;
	while(current->data.data[0] != ')')
	{
		if(current == NULL)
			semantic_error("Unexpected EOF encountered in function declaration", current->origin);
		statement *name = new(name);
		name->type = NAME;
		name->next = name->child = NULL;
		if(current->type != WORD)
			semantic_error("Parameter names must be a valid identifier", current->origin);
		name->data = current->data;
		current = current->next;
		if(current->type != SYMBOL || current->data.data[0] != ':')
			semantic_error("Parameters to functions must separate names and types with colons", current->origin);
		current = current->next;
		statement *param_type = new(param_type);
		if(current == NULL)
			semantic_error("Unexpected EOF encountered in function declaration", current->origin);
		if(current->type != WORD)
			semantic_error("Parameter types must be valid identifiers.", current->origin);
		param_type->data = current->data;
		param_type->type = TYPE;
		name->child = param_type;
		param_type->next = param_type->child = NULL;
		if(func->paramHead == NULL)
			func->paramHead = name;
		else if(func->paramTail == NULL)
		{
			func->paramTail = name;
			func->paramHead->next = name;
		}
		else
		{
			func->paramTail->next = name;
			func->paramTail = func->paramTail->next;
		}
		current = current->next;
		if(current->data.data[0] == ',')
			current = current->next;
	}
	current = current->next;
	func->type = current->data;
	current = current->next;
	if(current->data.data[0] != '{')
		semantic_error("Function bodies must start with an open brace ('{')", current->origin);
	statement *statement = new(statement);
	statement->next = statement->child = statement->parent = NULL;
	statement->type = ROOT;
	tokens->head = current;
	statement->child = get_expression(tokens);
	func->root = statement;
	tokens->head = tokens->head->next;
	return func;
}

static statement *get_expression(token_list *tokens)
{
	parse_token *current = tokens->head;
	if(current == NULL)
		semantic_error("Unexpected End of File", current->origin);
	statement *expression = new(expression);
	expression->child = expression->next = expression->parent = NULL;
	expression->data.data = NULL;
	expression->data.len = 0;
	switch(current->type)
	{
	case SYMBOL:
		if(current->data.data[0] == '{')
		{
			expression->type = BLOCK;
			tokens->head = tokens->head->next;
			token_list body = *tokens;
			int indent = 1;
			parse_token *current = body.head;
			while(indent > 0)
			{
				current = current->next;
				if(current->data.data[0] == '{')
					indent++;
				if(current->data.data[0] == '}')
					indent--;
			}
			body.tail = current;
			indent = 1;
			for(current = body.head; current != body.tail; current = current->next)
			{
				char character = current->data.data[0];
				if(character == '{')
					indent++;
				if(character == '}')
					indent--;
				if(indent == 1 && (character == '}' || character == ';'))
				{
					token_list item = body;
					item.tail = current;
					body.head = current->next;
					if(expression->child == NULL)
					{
						expression->child = get_expression(&item);
					} else
					{
						statement *currentExpression = expression->child;
						while(currentExpression->next != NULL)
						{
							currentExpression = currentExpression->next;
						}
						currentExpression->next = get_expression(&item);
					}
				}
			}
		} else if(current->data.data[0] == '}')
			return NULL;
		break;
	case WORD:
		if(tokens->head == tokens->tail)
		{
			if(equals_string(current->data, "break"))
			{
				expression->type = BREAK;
			} else if(equals_string(current->data, "continue"))
			{
				expression->type = CONTINUE;
			} else
			{
				expression->type = NAME;
				expression->data = current->data;
			}
		} else if(equals_string(current->data, "if") || equals_string(current->data, "while"))
		{
			if(equals_string(current->data, "if"))
				expression->type = IF;
			else
				expression->type = WHILE;
			expression->data = new_slice("");
			tokens->head = tokens->head->next;
			token_list header = *tokens;
			parse_token *end_of_header = header.head;
			while(end_of_header->next->data.data[0] != '{')
				end_of_header = end_of_header->next;
			header.tail = end_of_header;
			expression->child = get_expression(&header);
			tokens->head = header.tail->next;
			expression->child->next = get_expression(tokens);
		}
		break;
	case NUMBER:
		break;
	case STRING_LIT:
		break;
	case CHAR_LIT:
		break;
	}
	return expression;
}

static void semantic_error(char *error, source_origin origin)
{
	fprintf(stderr, "Error encountered while analyzing %s at line %d:\n%s\n", origin.filename, origin.line, error);
	exit(-1);
}
