#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "slice.h"
#include "ast.h"

ast_node* parse_toplevel(slice str);
ast_node* parse_function(slice data);
ast_node* parse_struct(slice data);
ast_node* parse(char* data);

int main()
{
	ast_node *root = parse("import x; using a; c getC(a : b) { return c{a}; } struct c{a:b;} import f;");
	printf("%s\n", to_string(root));
	return 0;
}

ast_node* parse(char* data) {
	int len = strlen(data);
	slice buffer = make_slice(data, 0, 0);
	int brace_level = 0;
	ast_node *root = new_node(ROOT, "");
	for(int i = 0; i < len; i++)
	{
		buffer.end += 1;
		if(data[i] == '{')
			brace_level += 1;
		else if(data[i] == '}')
		{
			brace_level -= 1;
			if(brace_level == 0)
			{
				slice start = buffer;
				start.begin++;
				start.end = start.begin + 6; //length of "struct"
				if(equals_string(start, "struct"))
					add_child(root, parse_struct(buffer));
				else
					add_child(root, parse_function(buffer));
				buffer.begin = buffer.end;
			}
		}
		else if(data[i] == ';' && brace_level == 0)
		{
			add_child(root, parse_toplevel(buffer));
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
		displace = strlen("import ");
	} else if(starts_with(data, new_slice("using")))
	{
		type = USING;
		displace = strlen("using ");
	}
	data.begin += displace;
	data.end--; //Remove semicolon
	ast_node *toplevel = new_node(type, evaluate(data));
	return toplevel;
}

void parse_block(slice data);
ast_node* parse_vartype(slice data);

ast_node* parse_function(slice data)
{
	while(is_whitespace(get(data, 0)))
		data.begin++;
	slice type = data;
	type.end = type.begin;
	while(!is_whitespace(get(type, type.end - type.begin)))
		type.end++;
	slice name = data;
	name.begin = type.end + 1;
	while(is_whitespace(get(name, 0)))
		name.begin++;
	name.end = name.begin;
	while(is_identifier(get(name, name.end - name.begin)))
		name.end++;
	ast_node *root, *returnType;
	root = new_node(FUNC, evaluate(name));
	returnType = new_node(TYPE, evaluate(type));
	add_child(root, returnType);
	slice vartype = name;
	vartype.begin = vartype.end = name.end + 1;
	while(get(vartype, -1) != '(')
		vartype.begin++;
	while(is_whitespace(get(vartype, 0)))
		vartype.begin++;
	vartype.end = vartype.begin + 1;
	while(get(vartype, 0) != ')' && get(vartype, vartype.end - vartype.begin) != ')')
	{
		vartype.end++;
		char current = get(vartype, vartype.end - vartype.begin);
		if(current == ';' || current == ')')
			add_child(root, parse_vartype(vartype));
	}
	slice block = vartype;
	block.begin = block.end;
	while(get(block, -1) != '{')
		block.begin++;
	int block_level = 0;
	block.end = block.begin;
	while(!(block_level == 0 && get(block, block.end - block.begin) == '}')) {
		char current = get(block, block.end - block.begin);
		if(current == '{')
			block_level++;
		if(current == '}') block_level--;
		block.end++;
	}
	parse_block(block);
	return root;
}

void parse_block(slice data)
{
	//TODO: STUB
	printf("BLOCK: %s\n", evaluate(data));
}

ast_node* parse_assignment(slice assign)
{
	slice data = assign;
	ast_type type;
	while(is_whitespace(get(data, 0)))
		data.begin++;
	ast_node *root;
	if(starts_with(data.data, new_slice("let")))
	{
		type = BIND;
		int i = 0;
		while(get(data, i) != '=')
		{
			if(get(data, i) == ':')
			{
				type = TYPED_BIND;
				break;
			}
		}
		data.begin += 4; //length of let and at least 1 whitespace token
		while(is_whitespace(get(data, 0)))
			data.begin++;
		data.end = data.begin+1;
		while(!is_whitespace(get(data, data.end - data.begin)) && get(data, data.end - data.begin) == ':')
			data.end++;
		char *name = evaluate(data);
		root = new_node(type, name);
		if(type == TYPED_BIND)
		{
			data.begin = data.end;
			while(get(data, -1) != ':')
				data.begin++;
			data.end = data.begin + 1;
			while(get(data, data.end - data.begin + 1) != '=')
				data.end++;
			char *typeStr = evaluate(data);
			data.begin = data.end + 1;
			data.end = assign.end;
			ast_node *typeNode = new_node(TYPE, typeStr);
			add_child(root, typeNode);
		}
	}
	else
	{
		type = ASSIGN;
		data.end = data.begin;
		while(get(data, data.end - data.begin + 1) != '=')
			data.end++;
		char *name = evaluate(data);
		root = new_node(type, name);
		data.begin = data.end + 2;
		data.end = assign.end;
	}
	//TODO: PARSE AND ADD RIGHT SIDE OF ASSIGNMENT
	return root;
}

ast_node* parse_call(slice data)
{
	//TODO: (STUB)
}

ast_node* parse_val(slice data)
{
	//TODO: (STUB)
}

ast_node* parse_struct(slice data)
{
	data.begin += 7;//length of struct and a space
	while(is_whitespace(get(data, 0)))
		data.begin++;
	slice structname = data;
	int i = 0;
	while(get(structname, i) != '{')
		i++;
	structname.end = structname.begin + i;
	ast_node *root = new_node(STRUCT, evaluate(structname));
	slice inner = structname;
	while(get(inner, -1) != '{')
		inner.begin++;
	inner.end = inner.begin;
	int len = strlen(inner.data);
	for(int i = inner.begin; i < len && data.data[i] != '}'; i++)
	{
		inner.end = i;
		if(get(inner, i - inner.begin) == ';')
		{
			ast_node *field = parse_vartype(inner);
			field->next = root->child;
			root->child = field;
			inner.begin = inner.end + 1;
		}
	}
	return root;
}

ast_node* parse_vartype(slice data)
{
	while(is_whitespace(get(data, 0)))
		data.begin++;
	slice varname;
	varname.data = data.data;
	varname.end = varname.begin = data.begin;
	int i = 0;
	while(get(varname, i) != ':') i++;
	varname.end += i;
	slice type;
	type.data = data.data;
	type.begin = varname.end + 1;
	type.end = data.end;
	char *varnameStr, *typeStr;
	varnameStr = evaluate(varname);
	typeStr = evaluate(type);
	ast_node *typeNode = new_node(TYPE, typeStr);
	ast_node *nameNode = new_node(VARTYPE, varnameStr);
	nameNode->child = typeNode;
	return nameNode;
}
