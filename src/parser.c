#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "slice.h"
#include "ast.h"

void parse_toplevel(slice str);
void parse_function(slice data);
void parse_struct(slice data);
void parse(char* data);

int main()
{
	ast_node *root=new_node(ROOT, "TEST");
	printf("%s\n", to_string(root));
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
