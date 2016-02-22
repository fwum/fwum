#include "symbols.h"
type_entry *get_type_table(ast_node *root)
{
	ast_node *current = root->child;
	type_entry *first = NULL;
	while(current != NULL)
	{
		
	}
}

func_entry *get_func_table(ast_node *root)
{

}
var_entry *get_all_vars(ast_node *block)
{

}
namespace_entry *get_namespace_table(ast_node *root)
{

}
void concat_type_table(type_entry *table1, type_entry *table2)
{

}
void concat_func_table(func_entry *table1, func_entry *table2)
{

}
void concat_var_table(var_entry *table1, var_entry *table2)
{

}
void concat_name_table(namespace_entry *table1, namespace_entry *table2)
{

}
void verify(ast_node *root, type_entry *typeTable, func_entry *funcTable, namespace_entry *namespace)
{

}
