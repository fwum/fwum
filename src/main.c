#include "parser.h"
#include "ast.h"
#include "cbackend.h"
#include <stdio.h>

int main()
{
	ast_node *root = parse("import x; using a; c getC(a : b) { if((a == b)) { let value = 2.1 + (call(param) + 3); } } struct c{a : int;} import f;");
	compile(root, stdout);
	return 0;
}
