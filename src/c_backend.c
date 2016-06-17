#include "c_backend.h"
#include <stdio.h>
#include "parser.h"

static void output_forward_declarations(file_contents contents, FILE *stream);
static void output_struct(struct_declaration *dec, FILE *stream);
static void output_func_header(func_declaration *dec, FILE *stream);
static void output_node(statement *expr, FILE *stream);
static void binary_op(statement *expr, char *operator, FILE *stream);

void output(file_contents contents, FILE *stream) {
    fprintf(stream, "#include <stdio.h>\n\
    #include <stdlib.h>\n \
    typedef unsigned char u8; \n \
    typedef char i8; \n \
    typedef unsigned short u16;\n \
    typedef short i16;\n \
    typedef unsigned u32; \n \
    typedef int i32;\n \
    typedef unsigned long u64; \n \
    typedef long i64; \n \
    typedef float f32;\n \
    typedef double f64;\n");
    output_forward_declarations(contents, stream);
    linked_iter structs = ll_iter_head(contents.structs);
    while(ll_iter_has_next(&structs)) {
        struct_declaration *dec = ll_iter_next(&structs);
        output_struct(dec, stream);
    }
    linked_iter funcs = ll_iter_head(contents.functions);
    while(ll_iter_has_next(&funcs)) {
        func_declaration *dec = ll_iter_next(&funcs);
        output_func_header(dec, stream);
        fprintf(stream, ";\n");
    }
    funcs = ll_iter_head(contents.functions);
    while(ll_iter_has_next(&funcs)) {
        func_declaration *dec = ll_iter_next(&funcs);
        output_func_header(dec, stream);
        fprintf(stream, "{");
        output_node(dec->root, stream);
        fprintf(stream, "}");
    }
}

static void output_forward_declarations(file_contents contents, FILE *stream) {
    linked_iter structs = ll_iter_head(contents.structs);
    while(ll_iter_has_next(&structs)) {
        struct_declaration *dec = ll_iter_next(&structs);
        char *name = evaluate(dec->name);
        fprintf(stream, "struct %s;\ntypedef struct %s %s;\n", name, name, name);
    }
}

static void output_struct(struct_declaration *dec, FILE *stream) {
    char *name = evaluate(dec->name);
    fprintf(stream, "struct %s {\n", name);
    linked_iter members = ll_iter_head(dec->members);
    while(ll_iter_has_next(&members)) {
        struct_member *mem = ll_iter_next(&members);
        fprintf(stream, "%s %s;\n", evaluate(mem->type), evaluate(mem->name));
    }
    fprintf(stream, "};\n");
}

static void output_func_header(func_declaration *dec, FILE *stream) {
    fprintf(stream, "%s %s(", evaluate(dec->type), evaluate(dec->name));
    linked_iter iterator = ll_iter_head(dec->parameters);
    while(ll_iter_has_next(&iterator)) {
        output_node(ll_iter_next(&iterator), stream);
        if(ll_iter_has_next(&iterator))
            fprintf(stream, ", ");
    }
    fprintf(stream, ")");
}

static void output_node(statement *expr, FILE *stream) {
    switch(expr->type) {
    case OP_ADD:
    	binary_op(expr, "+", stream);
        break;
    case OP_SUB:
        binary_op(expr, "-", stream);
        break;
    case OP_MULT:
        binary_op(expr, "*", stream);
        break;
    case OP_DIV:
        binary_op(expr, "/", stream);
        break;
    case OP_MOD:
        binary_op(expr, "%", stream);
        break;
    case OP_EXP:
        fprintf(stream, "pow(");
        output_node(ll_get_first(expr->children), stream);
        fprintf(stream, ",");
        output_node(ll_get_last(expr->children), stream);
        fprintf(stream, ")");
        break;
    case OP_BIT_AND:
    	binary_op(expr, "&", stream);
        break;
    case OP_BOOL_AND:
    	binary_op(expr, "&&", stream);
        break;
    case OP_BIT_OR:
    	binary_op(expr, "|", stream);
        break;
    case OP_BOOL_OR:
        binary_op(expr, "||", stream);
        break;
    case OP_BIT_XOR:
        binary_op(expr, "^", stream);
        break;
    case OP_BOOL_XOR:
    	fprintf(stream, "(!(");
        output_node(ll_get_first(expr->children), stream);
        fprintf(stream, ") != !(");
        output_node(ll_get_last(expr->children), stream);
        fprintf(stream, "))");
    case OP_BIT_NOT:
        binary_op(expr, "~", stream);
        break;
    case OP_BOOL_NOT:
        binary_op(expr, "!", stream);
        break;
    case OP_SHIFT_LEFT:
        binary_op(expr, "<<", stream);
        break;
    case OP_SHIFT_RIGHT:
        binary_op(expr, ">>", stream);
        break;
    case OP_BIT_SHIFT_RIGHT:
        binary_op(expr, ">>>", stream);
        break;
    case OP_GREATER:
        binary_op(expr, ">", stream);
        break;
    case OP_LESS:
        binary_op(expr, "<", stream);
        break;
    case OP_GREATER_EQUAL:
        binary_op(expr, ">=", stream);
        break;
    case OP_LESS_EQUAL:
        binary_op(expr, "<=", stream);
        break;
    case OP_EQUAL:
        binary_op(expr, "==", stream);
        break;
    case OP_NOT_EQUAL:
        binary_op(expr, "!=", stream);
        break;
    case OP_ASSIGN:
        binary_op(expr, "=", stream);
        break;
	case OP_INIT:
		output_node(ll_get_first(expr->children), stream);
        statement *name = ll_get_first(expr->children);
        fprintf(stream, "; %s=", evaluate(name->data));
        output_node(ll_get_last(expr->children), stream);
	break;
    case OP_MEMBER:
        binary_op(expr, ".", stream);
        break;
    case OP_GETREF:
        fprintf(stream, "&(");
        output_node(ll_get_first(expr->children), stream);
        break;
    case OP_DEREF:
        fprintf(stream, "*(");
        output_node(ll_get_first(expr->children), stream);
        break;
    case OP_INDEX:
    	output_node(ll_get_first(expr->children), stream);
        fprintf(stream, "[");
        output_node(ll_get_last(expr->children), stream);
        fprintf(stream, "]");
        break;
    case RETURN:
    	fprintf(stream, "return ");
        output_node(ll_get_first(expr->children), stream);
    break;
    case CONTINUE:
    	fprintf(stream, "continue");
    break;
    case BREAK:
        fprintf(stream, "break");
    break;
    case FUNC_CALL:
    	//TODO: FUNC CALL BACKEND
    break;
    case STACK_INIT:
    	//TODO: STACK INIT BACKEND
    break;
    case HEAP_INIT:
    	//TODO: HEAP INIT BACKEND
    break;
    case IF:
    	fprintf(stream, "if(");
        output_node(ll_get_first(expr->children), stream);
        fprintf(stream, ")");
        output_node(ll_get_last(expr->children), stream);
        break;
	case ELSE:
        fprintf(stream, "else");
        output_node(ll_get_first(expr->children), stream);
    	break;
	case ELSEIF:
        fprintf(stream, "else if(");
        output_node(ll_get_first(expr->children), stream);
        fprintf(stream, ")");
        output_node(ll_get_last(expr->children), stream);
        break;
    case WHILE:
        fprintf(stream, "while(");
        output_node(ll_get_first(expr->children), stream);
        fprintf(stream, ")");
        output_node(ll_get_last(expr->children), stream);
        break;
    case FOR:
    	//TODO: FOR BACKEND
    break;
    case FOREACH:
        fprintf(stream, "for(");
        output_node(ll_get_first(expr->children), stream);
        fprintf(stream, ")");
        output_node(ll_get_last(expr->children), stream);
        break;
    case BLOCK:
    	fprintf(stream, "{\n");
        linked_iter iterator = ll_iter_head(expr->children);
        while(ll_iter_has_next(&iterator)) {
            output_node(ll_iter_next(&iterator), stream);
            fprintf(stream, ";\n");
        }
        fprintf(stream, "}");
    break;
    case TYPE:
    	fprintf(stream, "%s", evaluate(expr->data));
    break;
    case NAME:
    	if(expr->children != NULL) {
            output_node(ll_get_first(expr->children), stream);
        }
        fprintf(stream, " %s", evaluate(expr->data));
    break;
    case ROOT:
        fprintf(stream, "{\n");
        iterator = ll_iter_head(expr->children);
        while(ll_iter_has_next(&iterator)) {
            output_node(ll_iter_next(&iterator), stream);
            fprintf(stream, ";\n");
        }
        fprintf(stream, "}");
    break;
    case STRING:
    	fprintf(stream, "(\"%s\")", evaluate(expr->data));
    break;
    case CHAR:
    	fprintf(stream, "('%s')", evaluate(expr->data));
    break;
    case NUM:
    	fprintf(stream, "(%s)", evaluate(expr->data));
    break;
    case VALUE_TRUE:
        fprintf(stream, "1");
    break;
    case VALUE_FALSE:
        fprintf(stream, "0");
    break;
	}
}

static void binary_op(statement *expr, char *operator, FILE *stream) {
    fprintf(stream, "(");
    output_node(ll_get_first(expr->children), stream);
    fprintf(stream, "%s", operator);
    output_node(ll_get_last(expr->children), stream);
    fprintf(stream, ")");
}
