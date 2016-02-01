#include "cbackend.h"
#include <stdio.h>
#include <stdlib.h>

void compile_struct(ast_node *structNode, FILE* stream);
void compile_vartype(ast_node *vartypeNode, FILE* stream);
void compile_func(ast_node *funcNode, FILE* stream);

void compile(ast_node *root, FILE* stream)
{
	for(ast_node *child = root->child; child != NULL; child = child->next)
	{
		switch(child->type)
		{
		case ROOT:
			fprintf(stderr, "Internal compiler error: root was passed as a non-root node to compile.");
			exit(-1);
		break;
		case IMPORT:
			fprintf(stderr, "Cannot import %s because imports are not yet implemented.", child->data);
		break;
		case USING:
			fprintf(stderr, "Cannot use %s because namespaces are not yet implemented.", child->data);
		break;
		case VARTYPE:
			fprintf(stderr, "Internal compiler error: vartype is not a valid top-level element.");
			exit(-1);
		break;
		case TYPE:
			fprintf(stderr, "Internal compiler error: type is not a valid top-level element.");
			exit(-1);
		break;
		case FUNC:
			compile_func(child, stdout);
		break;
		case BIND:
			fprintf(stderr, "Internal compiler error: bind is not a valid top-level element.");
			exit(-1);
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

void compile_func(ast_node *funcNode, FILE* stream)
{

}

void compile_vartype(ast_node *vartypeNode, FILE* stream)
{
	fprintf(stream, "%s %s", vartypeNode->child->data, vartypeNode->data);
}
