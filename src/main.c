#include "parser.h"
#include "ast.h"
#include "cbackend.h"
#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
	if(argc <= 1)
	{
		printf("No input files.\n");
		return -1;
	}
	else
	{
		FILE* input = fopen(argv[1], "r");
		char* data = read_file(input);
		fclose(input);
		ast_node *root = parse(data);
		free(data);
		if(argc >= 3 && strcmp(argv[2], "devel") == 0)
			printf("%s\n", to_string(root));
		compile(root, stdout);
		return 0;
	}
}
