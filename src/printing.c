#include <stdio.h>
#include "printing.h"
#include "ast_strings.h"

/*
Print an AST node to stdout (recursive)
*/
void dump_node(statement *state, int indentation);
static void print_token(parse_token *current);

/*
Print a list of tokens to stdout, mostly for debugging purposes
*/
void print_tlist(token_list list) {
    if(list.head == NULL) {
        return;
    }
    parse_token *current = list.head;
    while(current != list.tail) {
        print_token(current);
        current = current->next;
    }
    print_token(current);
}

static void print_token(parse_token *current) {
    switch(current->type) {
    case SYMBOL:
        printf("SYMBOL: %s\n", evaluate(current->data));
        break;
     case WORD:
        printf("WORD: %s\n", evaluate(current->data));
        break;
    case NUMBER:
        printf("NUMBER: %s\n", evaluate(current->data));
        break;
    case STRING_LIT:
        printf("STRING: %s\n", evaluate(current->data));
        break;
    case CHAR_LIT:
        printf("CHAR: %s\n", evaluate(current->data));
        break;
    }
}

/*
Print the created AST to stdout, mostly for debugging purposes
*/
void dump(file_contents contents) {
	{
		struct_declaration *current = contents.head;
		while(current != NULL) {
			printf("STRUCT: %s\n", evaluate(current->name));
			struct_member *member = current->head;
			while(member != NULL) {
				printf("\tMEMBER: NAME: %s | TYPE: %s\n", evaluate(member->name), evaluate(member->type));
				member = member->next;
			}
			current = current->next;
		}
	}
	{
		func_declaration *current = contents.funcHead;
		while(current != NULL) {
			printf("FUNC: %s | TYPE : %s\n", evaluate(current->name), evaluate(current->type));
			dump_node(current->paramHead, 0);
			dump_node(current->root, 0);
			current = current->next;
		}
	}

}

/*
Print an AST node to stdout (recursive)
*/
void dump_node(statement *state, int indentation) {
	if(state == NULL) {
		return;
    }
	for(int i = 0; i < indentation; i++) {
		printf("\t");
    }
    printf("%s", statement_to_string(state->type));
	printf(": %s\n", evaluate(state->data));
	if(state->child != NULL) {
		dump_node(state->child, indentation + 1);
    }
	if(state->next != NULL) {
        dump_node(state->next, indentation);
    }
}
