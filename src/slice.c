#include <stdlib.h>
#include <stdbool.h>
#include "slice.h"

bool equals(slice s1, slice s2)
{
	return s1.data == s2.data && s1.begin == s2.begin && s1.end == s2.end;
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
