#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

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
		printf("%s\n", argv[1]); //Make the compiler stop complaning about the definition of main
		//TODO: Build back up the main
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
		iterator = ll_iter_head(list);
		while(ll_iter_has_next(&iterator)) {
			ll_iter_next(&iterator);
		}
		ll_iter_clear_to_current(&iterator);
		test_assert(ll_empty(list), "Iterator clear to position");
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
	{
		start_test_set("Tokenizer");
		char *data = "word 1.2 \"string\" \n'c' word1 word! !";
		parse_source source = start_parse(data, "filename");
		parse_token token = get_token(&source);
		test_assert(token.type == WORD, "Word parse type");
		test_assert(equals_string(token.data, "word"), "Word parse value");
		test_assert(token.origin.line == 1, "Token line number #1");
		test_assert(strcmp(token.origin.filename, "filename") == 0, "Token file name");
		test_assert(has_token(source), "Has next with next");
		token = get_token(&source);
		test_assert(token.type == NUMBER, "Number parse type");
		test_assert(equals_string(token.data, "1.2"), "Number parse value");
		token = get_token(&source);
		test_assert(token.type == STRING_LIT, "String parse type");
		test_assert(equals_string(token.data, "string"), "String parse value");
		token = get_token(&source);
		test_assert(token.origin.line == 2, "Token line number #2");
		test_assert(token.type == CHAR_LIT, "Character parse type");
		test_assert(equals_string(token.data, "c"), "Character parse value");
		token = get_token(&source);
		test_assert(token.type == WORD, "Word parse type with number");
		test_assert(equals_string(token.data, "word1"), "Word parse value with number");
		token = get_token(&source);
		test_assert(token.type == WORD, "Word parse type with symbol");
		test_assert(equals_string(token.data, "word"), "Word parse value with symbol");
		token = get_token(&source);
		test_assert(token.type == SYMBOL, "Symbol parse type #1");
		test_assert(equals_string(token.data, "!"), "Symbol parse contents #1");
		token = get_token(&source);
		test_assert(token.type == SYMBOL, "Symbol parse type #2");
		test_assert(equals_string(token.data, "!"), "Symbol parse contents #2");
		test_assert(!has_token(source), "Has next without next");
		end_test_set();
	}
	printf("Total test results: %d passed, %d failed\n", all_tests.passed, all_tests.failed);
	return all_tests.failed != 0; //return an error code
}
#endif
