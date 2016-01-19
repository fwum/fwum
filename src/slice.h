#ifndef SLICE_H_
#define SLICE_H_
typedef struct slice
{
	char* data;
	int begin, end;
} slice;
bool equals(slice s1, slice s2);
bool equals_string(slice s1, char* data);
char* evaluate(slice s1);
#endif
