#include "cbackend.h"
#include <stdio.h>
void compile_struct(ast_node *structNode, FILE* stream);
void compile_vartype(ast_node *vartypeNode, FILE* stream);

void compile(ast_node *root, FILE* stream)
{
	for(ast_node *child = root->child; child != NULL; child = child->next)
	{
		switch(child->type)
		{
		case ROOT:
		break;
		case IMPORT:
		break;
		case USING:
		break;
		case VARTYPE:
		break;
		case TYPE:
		break;
		case FUNC:
		break;
		case BIND:
		break;
		case TYPED_BIND:
		break;
		case ASSIGN:
		break;
		case STRING:
		break;
		case NUMBER:
		break;
		case VAR:
		break;
		case CALL:
		break;
		case BLOCK:
		break;
		case OPERATOR:
		break;
		case CONTROL:
		break;
		case STRUCT:
			compile_struct(child, stream);
			break;
		}
	}
}

void compile_struct(ast_node *structNode, FILE* stream)
{
	fprintf(stream, "typedef struct %s {", structNode->data);
	ast_node *child = structNode->child;
	while(child != NULL)
	{
		compile_vartype(child, stream);
		child = child->next;
		fprintf(stream, "; \n");
	}
	fprintf(stream, "}%s;\n", structNode->data);
}

void compile_vartype(ast_node *vartypeNode, FILE* stream)
{
	fprintf(stream, "%s %s", vartypeNode->child->data, vartypeNode->data);
}
