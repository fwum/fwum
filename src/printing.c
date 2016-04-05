#include <stdio.h>
#include "printing.h"

/*
Print an AST node to stdout (recursive)
*/
static void dump_node(statement *state, int indentation);

/*
Print a list of tokens to stdout, mostly for debugging purposes
*/
void print_tlist(token_list list)
{
    parse_token *current = list.head;
    while(current != list.tail)
    {
        switch(current->type)
        {
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
        current = current->next;
    }
}

/*
Print the created AST to stdout, mostly for debugging purposes
*/
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
				printf("\tMEMBER: NAME: %s | TYPE: %s\n", evaluate(member->name), evaluate(member->type));
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
			dump_node(current->paramHead, 0);
			dump_node(current->root, 0);
			current = current->next;
		}
	}

}

/*
Print an AST node to stdout (recursive)
*/
static void dump_node(statement *state, int indentation)
{
	if(state == NULL)
		return;
	for(int i = 0; i < indentation; i++)
		printf("\t");
	switch(state->type)
	{
        case OP_ADD:
        	printf("OP_ADD");
        break;
        case OP_SUB:
        	printf("OP_SUB");
        break;
        case OP_MULT:
        	printf("OP_MULT");
        break;
        case OP_DIV:
        	printf("OP_DIV");
        break;
        case OP_MOD:
        	printf("OP_MOD");
        break;
        case OP_BIT_AND:
        	printf("OP_BIT_AND");
        break;
        case OP_BOOL_AND:
        	printf("OP_BOOL_AND");
        break;
        case OP_BIT_OR:
        	printf("OP_BIT_OR");
        break;
        case OP_BOOL_OR:
        	printf("OP_BOOL_OR");
        break;
        case OP_BIT_XOR:
        	printf("OP_BIT_XOR");
        break;
        case OP_BOOL_XOR:
        	printf("OP_BOOL_XOR");
        break;
        case OP_BIT_NOT:
        	printf("OP_BIT_NOT");
        break;
        case OP_BOOL_NOT:
        	printf("OP_BOOL_NOT");
        break;
        case OP_GREATER:
        	printf("OP_GREATER");
        break;
        case OP_LESS:
        	printf("OP_LESS");
        break;
        case OP_GREATER_EQUAL:
        	printf("OP_GREATER_EQUAL");
        break;
        case OP_LESS_EQUAL:
        	printf("OP_LESS_EQUAL");
        break;
        case OP_EQUAL:
        	printf("OP_EQUAL");
        break;
        case OP_NOT_EQUAL:
        	printf("OP_NOT_EQUAL");
        break;
        case OP_ASSIGN:
        	printf("OP_ASSIGN");
        break;
        case OP_MEMBER:
        	printf("OP_MEMBER");
        break;
        case OP_GETREF:
        	printf("OP_GETREF");
        break;
        case OP_DEREF:
        	printf("OP_DEREF");
        break;
        case OP_INDEX:
        	printf("OP_INDEX");
        break;
        case RETURN:
        	printf("RETURN");
        break;
        case CONTINUE:
        	printf("CONTINUE");
        break;
        case BREAK:
            printf("BREAK");
        break;
        case FUNC_CALL:
        	printf("FUNC_CALL");
        break;
        case STACK_INIT:
        	printf("STACK_INIT");
        break;
        case HEAP_INIT:
        	printf("HEAP_INIT");
        break;
        case IF:
        	printf("IF");
        break;
        case WHILE:
        	printf("WHILE");
        break;
        case FOR:
        	printf("FOR");
        break;
        case FOREACH:
        	printf("FOREACH");
        break;
        case BLOCK:
        	printf("BLOCK");
        break;
        case TYPE:
        	printf("TYPE");
        break;
        case NAME:
        	printf("NAME");
        break;
        case ROOT:
        	printf("ROOT");
        break;
        case STRING:
        	printf("STRING");
        break;
        case CHAR:
        	printf("CHAR");
        break;
        case NUM:
        	printf("NUM");
        break;
	}
	printf(": %s\n", evaluate(state->data));
	if(state->child != NULL)
		dump_node(state->child, indentation + 1);
	statement *current = state->next;
	while(current != NULL)
	{
		dump_node(current, indentation);
		current = current->next;
	}
}
