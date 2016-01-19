#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct slice
{
	char* data;
	int begin, end;
} slice;

void parse_toplevel(slice str);
void parse_function(slice data);
void parse_struct(slice data);
void parse(char* data);
bool equals(slice s1, slice s2);
bool equals_string(slice s1, char* data);

int main()
{
	parse("import x; a {potato} some more stuff; struct potato{}");
	return 0;
}



void parse(char* data) {
	int len = strlen(data);
	slice buffer;
	buffer.data = data;
	buffer.begin = 0;
	buffer.end = 0;
	int brace_level = 0;
	for(int i = 0; i < len; i++)
	{
		buffer.end += 1;
		if(data[i] == '{')
		{
			brace_level += 1;
		} else if(data[i] == '}')
		{
			brace_level -= 1;
			if(brace_level == 0)
			{
				slice start = buffer;
				start.end = start.begin + 6; //length of "struct"
				if(equals_string(start, "struct"))
					parse_struct(buffer);
				else
					parse_function(buffer);
				buffer.begin = buffer.end;
			}
		} else if(data[i] == ';' && brace_level == 0)
		{
			parse_toplevel(buffer);
			buffer.begin = buffer.end;
		}

	}
}

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

bool is_digit(char character)
{
	return character >= '0' && character <= '9';
}

bool is_whitespace(char character)
{
	return character == ' ' || character == '\t' || character == '\n' || character == '\r';
}

bool is_operator(char character)
{
	return character == '+' || character == '-' || character == '*' || character == '/';
}

void parse_toplevel(slice data)
{
	printf("TOPLEVEL: %s\n", evaluate(data));
}

void parse_function(slice data)
{
	printf("FUNC: %s\n", evaluate(data));
}

void parse_struct(slice data)
{
	printf("STRUCT: %s\n", evaluate(data));
}
