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

int main(int argc, char **argv) {
	#ifdef DO_TESTS
	int test_results = tests();
	if(test_results != 0) {
		return test_results;
	}
	#endif
	if(argc <= 1) {
		printf("No input files.\n");
		return -1;
	} else {
		FILE* input = fopen(argv[1], "r");
		char* data = read_file(input);
		fclose(input);
		token_list tokens = parse(data, argv[1]);
		token_list backup = tokens;
		file_contents contents = analyze(&tokens);
		parse_token *current = backup.head;
		while(current != NULL) {
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
#include "hashmap.h"
#include "stdio.h"
DEFSTRUCT(test_set);
struct test_set {
	char *name;
	int passed, failed;
};

static test_set current_test;
static test_set all_tests;

static int *box_int(int value) {
	int *ptr = new(ptr);
	*ptr = value;
	return ptr;
}

static void start_test_set(char *name) {
	printf("Test set: %s\n", name);
	current_test.name = name;
	current_test.passed = 0;
	current_test.failed = 0;
}

static void end_test_set() {
	all_tests.passed += current_test.passed;
	all_tests.failed += current_test.failed;
	printf("%s finished, with %d tests passing and %d tests failing.\n**********************\n", current_test.name, current_test.passed, current_test.failed);
}

static void test_assert(bool condition, char *test_name) {
	if(!condition) {
		printf("FAILED");
		current_test.failed += 1;
	} else {
		printf("PASSED");
		current_test.passed += 1;
	}
	printf(":\t%s\n", test_name);
}

static int tests() {
	{
		start_test_set("Test Framework");
		test_assert(true, "Test framework works");
		end_test_set();
	}
	{
		start_test_set("Linked List");
		linked_list *list = ll_new();
		test_assert(ll_empty(list), "Linked list empty function");
		ll_add_first(list, box_int(5));
		ll_add_first(list, box_int(6));
		linked_iter iterator = ll_iter_head(list);
		test_assert(*((int*)ll_iter_next(&iterator)) == 6, "Linked list iterator");
		ll_clear(list);
		test_assert(ll_empty(list), "Linked list clear function");
		ll_destroy(list);
		end_test_set();
	}
	{
		start_test_set("Hash map");
		hash_map *map = hm_new();
		for(int i = 0; i < 10; i++)
			hm_put(map, i, box_int(i * i));
		test_assert(*((int*)hm_get(map, 5)) == 25, "Hash map put and get");
		test_assert(!hm_has(map, 11), "Hash map contains");
		end_test_set();
	}
	printf("Total test results: %d passed, %d failed\n", all_tests.passed, all_tests.failed);
	return all_tests.failed != 0; //return an error code
}
#endif
