#include <stdio.h>
#include "printing.h"
#include "ast_strings.h"

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
        linked_iter iterator = ll_iter_head(contents.imports);
        while(ll_iter_has_next(&iterator)) {
            import_declaration *import = ll_iter_next(&iterator);
            printf("IMPORT: %s\n", evaluate(import->name));
        }
    }
    {
        linked_iter iterator = ll_iter_head(contents.enums);
        while(ll_iter_has_next(&iterator)) {
            enum_declaration *dec = ll_iter_next(&iterator);
            printf("ENUM: %s\n", evaluate(dec->name));
            linked_iter items = ll_iter_head(dec->options);
            while(ll_iter_has_next(&items)) {
                statement *option = ll_iter_next(&items);
                printf("\tOPTION: %s\n", evaluate(option->data));
            }
        }
    }
	{
		linked_iter iterator = ll_iter_head(contents.structs);
		while(ll_iter_has_next(&iterator)) {
            struct_declaration *current = ll_iter_next(&iterator);
			printf("STRUCT: %s\n", evaluate(current->name));
            linked_iter iterator = ll_iter_head(current->members);
			while(ll_iter_has_next(&iterator)) {
                struct_member *member = ll_iter_next(&iterator);
				printf("\tMEMBER: NAME: %s | TYPE: %s\n", evaluate(member->name), evaluate(member->type));
			}
		}
	}
	{
        linked_iter iterator = ll_iter_head(contents.functions);
        while(ll_iter_has_next(&iterator)) {
            func_declaration *current = ll_iter_next(&iterator);
			printf("FUNC: %s | TYPE : %s\n", evaluate(current->name), evaluate(current->type));
            printf("\tPARAMETERS:\n");
			dump_node(current->parameters, 2);
            printf("\tBODY:\n");
			dump_node(current->root->children, 2);
		}
	}

}

/*
Print an AST node to stdout (recursive)
*/
void dump_node(linked_list *list, int indentation) {
    linked_iter iterator = ll_iter_head(list);
    while(ll_iter_has_next(&iterator)) {
        for(int i = 0; i < indentation; i++) {
    		printf("\t");
        }
        statement *state = ll_iter_next(&iterator);
        printf("%s", statement_to_string(state->type));
    	printf(": %s\n", evaluate(state->data));
    	if(state->children != NULL) {
    		dump_node(state->children, indentation + 1);
        }
    }
}

void print_tokens(parse_source source) {
    optional op = get_token(&source);
    while(op_has(op)) {
        parse_token *token = op_get(op);
        print_token(token);
        op = get_token(&source);
    }
}
