#include "cbackend.h"
#include "ast.h"
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
		case FUNC:
			compile_func(child, stdout);
		break;
		case STRUCT:
			compile_struct(child, stream);
			break;
		default:
			fprintf(stderr, "Internal compiler error: %s is not a valid top-level element.\n", type_to_string(child->type));
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
