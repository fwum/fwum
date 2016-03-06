#ifndef SLICE_H_
#define SLICE_H_
#include <stdbool.h>
typedef struct slice
{
	char* data;
	int len;
} slice;
slice new_slice(char* string);
slice make_slice(char* string, int length);
bool equals(slice s1, slice s2);
bool equals_string(slice s1, char* data);
bool starts_with(slice s1, slice s2);
bool slice_contains(slice s1, char c);
char* evaluate(slice s1);
#endif
