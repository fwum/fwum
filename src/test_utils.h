#ifndef TEST_UTILS_H_
#define TEST_UTILS_H_
#include <stdio.h>

#define ASSERT(value, test_name) do { printf("Test:\t%s\t", (test_name)); \
	if(!(value)) { printf("FAILED\n"); exit(-1); } else { printf("PASSED\n"); } } \
	while(0);
#endif
