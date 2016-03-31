#include "tokenizer.h"
#include "semantic_analyzer.h"
#include "io.h"
#include "printing.h"
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
		token_list tokens = parse(data, argv[1]);
		token_list backup = tokens;
		file_contents contents = analyze(&tokens);
		parse_token *current = backup.head;
		while(current != NULL)
		{
			free(current);
			current = current->next;
		}
		dump(contents);
		print_tlist(tokens);
		return 0;
	}
}
