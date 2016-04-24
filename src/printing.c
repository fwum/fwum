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
void print_tlist(linked_list *list) {
    if(ll_empty(list)) {
        return;
    }
    linked_iter iterator = ll_iter_head(list);
    parse_token *current = ll_iter_next(&iterator);
    while(ll_iter_has_next(&iterator)) {
        print_token(current);
        current = ll_iter_next(&iterator);
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
		linked_iter iterator = ll_iter_head(contents.structs);
		while(ll_iter_has_next(&iterator)) {
            struct_declaration *current = ll_iter_next(&iterator);
			printf("STRUCT: %s\n", evaluate(current->name));
			struct_member *member = current->head;
			while(member != NULL) {
				printf("\tMEMBER: NAME: %s | TYPE: %s\n", evaluate(member->name), evaluate(member->type));
				member = member->next;
			}
		}
	}
	{
        linked_iter iterator = ll_iter_head(contents.functions);
        while(ll_iter_has_next(&iterator)) {
            func_declaration *current = ll_iter_next(&iterator);
			printf("FUNC: %s | TYPE : %s\n", evaluate(current->name), evaluate(current->type));
			dump_node(current->paramHead, 0);
			dump_node(current->root, 0);
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
