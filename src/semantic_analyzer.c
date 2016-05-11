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
static statement *get_expression(linked_list *tokens);
static statement *parse_operation(linked_list *tokens);
static struct_declaration *analyze_struct(parse_source *source);
static linked_list *create_list(parse_source *source);
static statement *parse_body(parse_source *source);

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
	func->body = ll_new();
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
		if(current.type != SYMBOL || current.data.data[0] != ':') {
			semantic_error("Parameters to functions must separate names and types with colons", current.origin);
		}
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
	current = get_mandatory_token(source);
	if(current.data.data[0] != '{') {
		semantic_error("Function bodies must start with an open brace ('{')", current.origin);
	}
	statement *state = new(state);
	state->type = ROOT;
	ll_add_first(state->children, parse_body(source));
	ll_add_first(func->body, state);
	get_mandatory_token(source);
	return func;
}

static linked_list *create_list(parse_source *source) {
	parse_token *heap = new(heap);
	linked_list *list = ll_new();
	parse_token token = get_mandatory_token(source);
	int bracket_level = 0;
	while(true) {
		if(equals_string(token.data, "{")) {
			bracket_level += 1;
		} else if(equals_string(token.data, "}")) {
			bracket_level -= 1;
		}
		if(bracket_level == 0 && (equals_string(token.data, "}") || equals_string(token.data, ";")))
			break;
		*heap = token;
		ll_add_last(list, heap);
		heap = new(heap);
		token = get_mandatory_token(source);
	}
	return list;
}

static statement *parse_body(parse_source *source) {
	return get_expression(create_list(source));
}

static statement *get_expression(linked_list *tokens) {
	linked_iter iterator = ll_iter_head(tokens);
	parse_token *current = ll_iter_next(&iterator);
	statement *expression = new(expression);
	expression->children = NULL;
	expression->data.data = NULL;
	expression->data.len = 0;
	//TODO: Consider removal
	bool foundPattern = false;
	parse_token *last = ll_get_last(tokens);
	if(last->data.data[0] == ';') {
		ll_remove_last(tokens);
	}
	parse_token *first = ll_get_last(tokens);
	last = ll_get_last(tokens);
	while(first->data.data[0] == '(' && last->data.data[0] == ')') {
		ll_remove_first(tokens);
		ll_remove_last(tokens);
	}
	iterator = ll_iter_head(tokens);
	current = ll_iter_next(&iterator);
	parse_token *next = NULL;
	switch(current->type) {
	case SYMBOL:
		if(current->data.data[0] == '{') {
			foundPattern = true;
			expression->type = BLOCK;
			ll_remove_first(tokens);
			linked_list *body = ll_duplicate(tokens);
			int indent = 1;
			linked_iter iterator = ll_iter_head(body);
			parse_token *current = ll_iter_next(&iterator);
			while(indent > 0) {
				current = ll_iter_next(&iterator);
				if(current->data.data[0] == '{') {
					indent++;
				} else if(current->data.data[0] == '}') {
					indent--;
				}
			}
			ll_iter_clear_remaining(&iterator);
			indent = 1;
			iterator = ll_iter_head(body);
			for(current = ll_iter_next(&iterator); ll_iter_has_next(&iterator); current = ll_iter_next(&iterator)) {
				char character = current->data.data[0];
				if(character == '{') {
					indent++;
				} else if(character == '}') {
					indent--;
				}
				if(indent == 1 && (character == '}' || character == ';')) {
					linked_list *item = ll_duplicate(body);
					while(ll_get_last(item) != current)
						ll_remove_last(item);
					ll_iter_clear_to_current(&iterator);
					if(expression->children == NULL)
						expression->children = ll_new();
					ll_add_last(expression->children, item);
					ll_destroy(item);
				}
			}
			while(ll_get_first(tokens) != ll_get_last(body))
				ll_remove_first(tokens);
			ll_destroy(body);
		} else if(current->data.data[0] == '}') {
			return NULL;
		}
		break;
	case WORD:
		if(ll_get_first(tokens) == ll_get_last(tokens)) {
			foundPattern = true;
			if(equals_string(current->data, "break")) {
				expression->type = BREAK;
			} else if(equals_string(current->data, "continue")) {
				expression->type = CONTINUE;
			} else {
				expression->type = NAME;
				expression->data = current->data;
			}
		} else if(equals_string(current->data, "if") || equals_string(current->data, "while")) {
			foundPattern = true;
			if(equals_string(current->data, "if")) {
				expression->type = IF;
			} else {
				expression->type = WHILE;
			}
			expression->data = new_slice("");
			ll_remove_first(tokens);
			linked_list *header = ll_duplicate(tokens);
			linked_iter end_of_header_iter = ll_iter_head(header);
			while(((parse_token*)ll_iter_next(&end_of_header_iter))->data.data[0] != '{') {}
			ll_iter_clear_remaining(&end_of_header_iter);
			if(expression->children == NULL)
				expression->children = ll_new();
			ll_add_first(expression->children, get_expression(header));
			while(ll_get_first(tokens) != ll_get_last(header))
				ll_remove_first(tokens);
			ll_remove_first(tokens);
			ll_add_last(expression->children, get_expression(tokens));
		} else if((next = ll_iter_next(&iterator))->data.data[0] == '(') {
			foundPattern = true;
			expression->type = FUNC_CALL;
			expression->data = current->data;
			int paren_level = 0;
			linked_list *list = tokens;
			current = next;
			while(ll_get_first(list) != current)
				ll_remove_first(list);
			for(current = ll_iter_next(&iterator); paren_level != -1; current = ll_iter_next(&iterator)) {
				char currentChar = current->data.data[0];
				if((currentChar == ')' || currentChar == ',') && paren_level == 0) {
					linked_list *param = ll_duplicate(list);
					while(ll_get_last(param) != current)
						ll_remove_last(param);
					ll_remove_last(param);
					while(ll_get_first(list) != ll_get_last(param))
						ll_remove_first(list);
					ll_remove_first(list);
					ll_remove_first(list);
					if(expression->children == NULL)
						expression->children = ll_new();
					ll_add_last(expression->children, get_expression(param));
				}
				if(currentChar == '(') {
					paren_level += 1;
				} else if(currentChar == ')') {
					paren_level -= 1;
				}
			}
			//TODO: UNCOMMENT AND FIGURE OUT ISSUE
			//expression->next = get_expression(&list);
			while(ll_get_first(tokens) != ll_get_last(list))
				ll_remove_first(tokens);
			ll_remove_first(tokens);
		}
		break;
	case NUMBER:
		if(ll_get_first(tokens) == ll_get_last(tokens)) {
			foundPattern = true;
			expression->type = NUM;
			parse_token *first = ll_get_first(tokens);
			expression->data = first->data;
		}
		break;
	case STRING_LIT:
		if(ll_get_first(tokens) == ll_get_last(tokens)) {
			foundPattern = true;
			expression->type = STRING;
			parse_token *first = ll_get_first(tokens);
			expression->data = first->data;
		}
		break;
	case CHAR_LIT:
		if(ll_get_first(tokens) == ll_get_last(tokens)) {
			foundPattern = true;
			expression->type = CHAR;
			parse_token *first = ll_get_first(tokens);
			expression->data = first->data;
		}
		break;
	}
	if(!foundPattern) {
		expression = parse_operation(tokens);
	}
	return expression;
}

static statement *parse_operation(linked_list *tokens) {
	linked_iter iterator = ll_iter_head(tokens);
	linked_list *operator = get_node();
	linked_iter level = ll_iter_head(operator);
	while(ll_iter_has_next(&level)) {
		int paren_level = 0;
		linked_list *currentLevel = ll_iter_next(&level);
		for(parse_token *current = ll_iter_next(&iterator); ll_iter_has_next(&iterator); current = ll_iter_next(&iterator)) {
			char currentChar = current->data.data[0];
			if(currentChar == '(') {
				paren_level += 1;
			} else if(currentChar == ')') {
				paren_level -= 1;
			}
			if(paren_level == 0) {
				linked_iter innerMost = ll_iter_head(currentLevel);
				while(ll_iter_has_next(&innerMost)) {
					operator_node *currentOperator = ll_iter_next(&innerMost);
					if(equals_string(current->data, currentOperator->data)) {
						linked_list *op1 = ll_duplicate(tokens);
						while(ll_get_last(op1) != current)
							ll_remove_last(op1);
						ll_remove_last(op1);
						linked_list *op2 = tokens;
						while(ll_get_first(op2) != current)
							ll_remove_first(op2);
						ll_remove_first(op2);
						ll_remove_first(op2);
						statement *expression = new(expression);
						expression->data = new_slice("");
						expression->type = currentOperator->operatorType;
						expression->children = ll_new();
						ll_add_last(expression->children, get_expression(op1));
						ll_add_last(expression->children, get_expression(op2));
						return expression;
					}
				}
			}
		}
	}
	return NULL;
}

static void semantic_error(char *error, source_origin origin) {
	fprintf(stderr, "Error encountered while analyzing %s at line %d:\n%s\n", origin.filename, origin.line, error);
	exit(-1);
}
