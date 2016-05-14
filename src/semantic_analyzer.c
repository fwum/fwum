#include "semantic_analyzer.h"
#include "slice.h"
#include "util.h"
#include "printing.h"
#include "operators.h"
#include "optional.h"
#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

static void semantic_error(char *error, source_origin origin);
static func_declaration *analyze_func(parse_source *source);
static statement *get_expression(parse_source *source, int *indent);
static struct_declaration *analyze_struct(parse_source *source);

file_contents analyze(parse_source source) {
	file_contents contents;
	contents.structs = ll_new();
	contents.functions = ll_new();
	optional op = get_token(&source);
	while(op_has(op)) {
		parse_token current = *((parse_token*)op_get(op));
		if(equals(current.data, new_slice("struct"))) {
			struct_declaration *dec = analyze_struct(&source);
			ll_add_last(contents.structs, dec);
		} else if(equals(current.data, new_slice("func"))) {
			func_declaration *func = analyze_func(&source);
			ll_add_last(contents.functions, func);
		}
		op = get_token(&source);
	}
	return contents;
}

static struct_declaration *analyze_struct(parse_source *source) {
	parse_token current = get_mandatory_token(source);
	struct_declaration *dec = new(dec);
	dec->name = current.data;
	dec->members = ll_new();
	current = get_mandatory_token(source);
	if(current.data.data[0] != '{') {
		semantic_error("Expected opening brace after name in struct declaration.", current.origin);
	}
	current = get_mandatory_token(source);
	while(current.data.data[0] != '}')	{
		struct_member *member = new(member);
		if(current.type != WORD) {
			semantic_error("Struct members must be declared as <value> <type>;", current.origin);
		}
		member->name = current.data;
		current = get_mandatory_token(source);
		if(current.type != WORD) {
			semantic_error("Struct members must be declared as <value> <type>;",current.origin);
		}
		member->type = current.data;
		current = get_mandatory_token(source);
		if(current.data.data[0] != ';') {
			semantic_error("Struct members must be declared as <value> <type>;", current.origin);
		}
		ll_add_last(dec->members, member);
		current = get_mandatory_token(source);
	}
	return dec;
}

static func_declaration *analyze_func(parse_source *source) {
	parse_token current = get_mandatory_token(source);
	func_declaration *func = new(func);
	func->parameters = ll_new();
	if(current.type != WORD) {
		semantic_error("Function declaration must be in the form func <name>(<parameters>) <returntype> {<block>}", current.origin);
	}
	func->name = current.data;
	current = get_mandatory_token(source);
	if(current.data.data[0] != '(') {
		semantic_error("Function name must be followed by an open parenthesis", current.origin);
	}
	current = get_mandatory_token(source);
	while(current.data.data[0] != ')') {
		statement *name = new(name);
		name->type = NAME;
		name->children = ll_new();
		if(current.type != WORD) {
			semantic_error("Parameter names must be a valid identifier", current.origin);
		}
		name->data = current.data;
		current = get_mandatory_token(source);
		statement *param_type = new(param_type);
		if(current.type != WORD) {
			semantic_error("Parameter types must be valid identifiers.", current.origin);
		}
		param_type->data = current.data;
		param_type->type = TYPE;
		ll_add_first(name->children, param_type);
		param_type->children = NULL;
		ll_add_last(func->parameters, name);
		current = get_mandatory_token(source);
		if(current.data.data[0] == ',') {
			current = get_mandatory_token(source);
		}
	}
	current = get_mandatory_token(source);
	func->type = current.data;
	current = peek_mandatory_token(source);
	if(current.data.data[0] != '{') {
		semantic_error("Function bodies must start with an open brace ('{')", current.origin);
	}
	int indent = 1;
	get_mandatory_token(source);
	func->root = new(func->root);
	func->root->type = ROOT;
	func->root->data = new_slice("");
	func->root->children = ll_new();
	while(indent > 0) {
		statement *state = get_expression(source, &indent);
		if(state != NULL)
			ll_add_last(func->root->children, state);
	}
	return func;
}


static statement *get_expression(parse_source *source, int *indent) {
	statement *expression = new(expression);
	expression->children = NULL;
	expression->data.data = NULL;
	expression->data.len = 0;
	parse_token token = get_mandatory_token(source);
	if(equals_string(token.data, "{")) {
		expression->type = BLOCK;
		expression->children = ll_new();
		int finished = *indent - 1;
		*indent += 1;
		while(*indent != finished) {
			statement *state = get_expression(source, indent);
			if(state != NULL)
				ll_add_last(expression->children, state);
		}
	} else if(equals_string(token.data, "}")) {
		*indent -= 1;
		free(expression);
		return NULL;
	} else if(equals_string(token.data, "if") || equals_string(token.data, "while")
		|| equals_string(token.data, "for")) {
		if(equals_string(token.data, "if"))
			expression->type = IF;
		else if(equals_string(token.data, "while"))
			expression->type = WHILE;
		else if(equals_string(token.data, "for"))
			expression->type = FOR;
		expression->children = ll_new();
		ll_add_last(expression->children, get_expression(source, indent)); //Add the header
		ll_add_last(expression->children, get_expression(source, indent)); //Add the body
	} else {
		expression->data = token.data;
		switch(token.type) {
		case WORD:
			expression->type = NAME;
			break;
		case NUMBER:
			expression->type = NUM;
			break;
		case STRING_LIT:
			expression->type = STRING;
			break;
		case CHAR_LIT:
			expression->type = CHAR;
			break;
		case SYMBOL:
			printf("Symbol case in get_expression should not have executed\n");
			break;
		}
	}
	return expression;
}

static void semantic_error(char *error, source_origin origin) {
	fprintf(stderr, "Error encountered while analyzing %s at line %d:\n%s\n", origin.filename, origin.line, error);
	exit(-1);
}
