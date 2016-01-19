#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "slice.h"

slice new_slice(char* string)
{
	return make_slice(string, 0, strlen(string));
}

slice make_slice(char* string, int begin, int end)
{
	slice s;
	s.data = string;
	s.begin = begin;
	s.end = end;
	return s;
}

bool equals(slice s1, slice s2)
{
	return s1.data == s2.data && s1.begin == s2.begin && s1.end == s2.end;
}

bool starts_with(slice s1, slice s2)
{
	if(s1.end - s1.begin < s2.end - s2.begin) return false;
	for(int i = 0; i < s2.end - s2.end; i++)
		if(s1.data[s1.begin + i] != s2.data[s2.begin + i])
			return false;
	return true;
}

bool equals_string(slice s1, char* data)
{
	for(int i = 0; i < s1.end - s1.begin; i++)
		if(s1.data[i + s1.begin] != data[i])
			return false;
	return true;
}

char* evaluate(slice str)
{
	char* string = malloc(sizeof(*string) * (str.end - str.begin + 1));
	int i = 0;
	for(; i < str.end - str.begin; i++)
	{
		string[i] = str.data[i + str.begin];
	}
	string[i] = '\0';
	return string;
}
