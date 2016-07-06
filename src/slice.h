#ifndef SLICE_H_
#define SLICE_H_
#include <stdbool.h>
#include "util.h"
#include "optional.h"
DEFSTRUCT(slice);
/*
Create a slice of a string for much faster string manipulation
Use pointer math to offset the beginning
*/
struct slice {
	char* data;
	int len;
};
/*
Create a new slice that is identical to the parameter string
*/
slice new_slice(char* string);
/*
Create a new slice that starts with the provided pointer with the given length
*/
slice make_slice(char* string, int length);
/*
Checks if every character in each slice matches to the same character in the same position
*/
bool equals(slice s1, slice s2);
/*
Checks if every character in the slice matches every character in the string
*/
bool equals_string(slice s1, char* data);
/*
Checks if s1 starts with s2
*/
bool starts_with(slice s1, slice s2);
/*
Checks to see if the slice contains the character
*/
bool slice_contains(slice s1, char c);
/*
Checks to see if a slice is a numeric value
*/
bool is_numeric(slice s);
/*
Convert the slice to a string separate from the source
Useful if the string will be destroyed or the slice must be printed to stdout
*/
char* evaluate(slice s1);
/*
Gets a hash value for the slice
Useful for insertion into the hashmap
*/
int slice_hash(slice s);
/*
Parses an int from a slice
*/
optional parse_int(slice s);
/*
Useful for hashmap, mostly
*/
bool slice_eq_voidptr(void *slice1, void *slice2);
/*
Convert an integer into a slice
*/
slice int_to_slice(int number);
#endif
