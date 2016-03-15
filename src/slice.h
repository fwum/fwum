#ifndef SLICE_H_
#define SLICE_H_
#include <stdbool.h>
#include "util.h"
DEFSTRUCT(slice);
/*
Create a slice of a string for much faster string manipulation
Use pointer math to offset the beginning
*/
struct slice
{
	char* data;
	int len;
};
slice new_slice(char* string);
slice make_slice(char* string, int length);
bool equals(slice s1, slice s2);
bool equals_string(slice s1, char* data);
bool starts_with(slice s1, slice s2);
bool slice_contains(slice s1, char c);
char* evaluate(slice s1);
#endif
