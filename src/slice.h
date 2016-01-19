#ifndef SLICE_H_
#define SLICE_H_
typedef struct slice
{
	char* data;
	int begin, end;
} slice;
slice new_slice(char* string);
slice make_slice(char* string, int start, int end);
bool equals(slice s1, slice s2);
bool equals_string(slice s1, char* data);
bool starts_with(slice s1, slice s2);
char* evaluate(slice s1);
bool is_whitespace(char c);
char get(slice s, int position);
#endif
