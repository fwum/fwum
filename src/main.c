#include "tokenizer.h"
#include "semantic_analyzer.h"
#include "io.h"
#include "printing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef DO_TESTS
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
DEFSTRUCT(test_set);
struct test_set {
	char *name;
	int passed, failed;
};

#define DO_TEST_SET(input_name) do {char *name = (input_name); printf("Test set: %s\n", name); current_test.name = name; current_test.passed = 0; current_test.failed = 0; } while(0);
#define END_TEST_SET() printf("%s finished, with %d tests passing and %d tests failing.\n**********************\n", current_test.name, current_test.passed, current_test.failed);

#define ASSERT(value, test_name) do { if(!(value)) { printf("FAILED"); current_test.failed += 1; } else { printf("PASSED"); current_test.passed += 1;} \
	printf(":\t%s\n", test_name);} \
	while(0);

static test_set current_test;

static int *box_int(int value)
{
	int *ptr = new(ptr);
	*ptr = value;
	return ptr;
}

static int tests()
{
	DO_TEST_SET("Test Framework");
	ASSERT(true, "Test framework works");
	END_TEST_SET()
	DO_TEST_SET("Linked List");
	linked_list list = {NULL, NULL};
	ASSERT(ll_empty(&list), "Linked list empty function");
	ll_add_first(&list, box_int(5));
	ll_add_first(&list, box_int(6));
	linked_iter iterator = ll_iter_head(&list);
	ASSERT(*((int*)ll_iter_next(&iterator)) == 6, "Linked list iterator");
	ll_clear(&list);
	ASSERT(ll_empty(&list), "Linked list clear function");
	END_TEST_SET();
	return 0;
}
#endif
