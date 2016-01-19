#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "slice.h"
#include "ast.h"

ast_node* parse_toplevel(slice str);
void parse_function(slice data);
void parse_struct(slice data);
ast_node* parse(char* data);

int main()
{
	ast_node *root = parse("import x; using a;");
	printf("%s\n", to_string(root));
	return 0;
}

ast_node* parse(char* data) {
	int len = strlen(data);
	slice buffer;
	buffer.data = data;
	buffer.begin = 0;
	buffer.end = 0;
	int brace_level = 0;
	ast_node *root = new_node(ROOT, "");
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
			ast_node *top = parse_toplevel(buffer);
			top->next = root->child;
			root->child = top;
			buffer.begin = buffer.end;
		}
	}
	return root;
}

ast_node* parse_toplevel(slice data)
{
	ast_type type;
	int displace = 0;
	if(starts_with(data, new_slice("import")))
	{
		type = IMPORT;
		displace = strlen("import");
	} else if(starts_with(data, new_slice("using")))
	{
		type = USING;
		displace = strlen("using");
	}
	data.begin += displace;
	ast_node *toplevel = new_node(type, evaluate(data));
	return toplevel;
}

void parse_function(slice data)
{
	printf("FUNC: %s\n", evaluate(data));
}

void parse_struct(slice data)
{
	printf("STRUCT: %s\n", evaluate(data));
}
