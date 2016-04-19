#include "tokenizer.h"
#include "semantic_analyzer.h"
#include "io.h"
#include "printing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef DO_TESTS
#include "test_utils.h"
static int tests();
#endif

int main(int argc, char **argv)
{
	#ifdef DO_TESTS
	int test_results = tests();
	if(test_results != 0)
		return test_results;
	#endif
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

#ifdef DO_TESTS
#include "linked_list.h"
#include "stdio.h"
static int *box_int(int value)
{
	int *ptr = new(ptr);
	*ptr = value;
	return ptr;
}

static int tests()
{
	ASSERT(true, "Test framework works");
	linked_list list = {NULL, NULL};
	ll_add_first(&list, box_int(5));
	ll_add_first(&list, box_int(6));
	linked_iter iterator = ll_iter_head(&list);
	ASSERT(*((int*)ll_iter_next(&iterator)) == 6, "Linked list iterator");
	
	return 0;
}
#endif
