#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
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

slice clone_slice(slice s, int beginNew, int endNew)
{
	return make_slice(s.data, beginNew, endNew);
}

bool equals(slice s1, slice s2)
{
	return s1.data == s2.data && s1.begin == s2.begin && s1.end == s2.end;
}

bool starts_with(slice s1, slice s2)
{
	//if(s1.end - s1.begin < s2.end - s2.begin) return false;
	for(int i = 0; i < s2.end - s2.begin; i++)
	{
		if(get(s1, i) != get(s2, i))
			return false;
	}
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
	if (str.end < str.begin)
	{
		fprintf(stderr, "Evaluate was passed an end value less than its begin value.");
		exit(-1);
		return "";
	}
	int i = 0;
	for(; i < str.end - str.begin; i++)
	{
		string[i] = str.data[i + str.begin];
	}
	string[i] = '\0';
	return string;
}

char get(slice s1, int pos)
{
	return s1.data[s1.begin + pos];
}

char get_last(slice s)
{
	return get(s, s.end - s.begin);
}

bool is_whitespace(char c)
{
	return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

bool is_identifier(char c)
{
	return (c >= 'a' && c <= 'z') || (c <= 'A' && c >= 'Z')  || (c >= '0' && c <= '9') || c == '_';
}
