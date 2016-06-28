#include "semantic_analyzer.h"
#include "parser.h"
#include "symbol_table.h"
#include "types.h"
#include "util.h"

static type get_node_type(symbol_table *context, statement *expr);
static void analyze_node(symbol_table *context, statement *node);
static type leftmost_type(symbol_table *context, statement *expr);
static type rightmost_type(symbol_table *context, statement *expr);

void analyze(file_contents contents) {
    linked_iter iterator = ll_iter_head(contents.functions);
    symbol_table *funcReturns = st_new();
    while(ll_iter_has_next(&iterator)) {
        func_declaration *dec = ll_iter_next(&iterator);
        type returnType = get_type(contents, dec->type);
        type *boxedType = new(boxedType);
        *boxedType = returnType;
        slice *boxedSlice = new(boxedSlice);
        *boxedSlice = dec->name;
        st_put(funcReturns, boxedSlice, boxedType);
        dec->type = type_to_string(returnType);
    }
    iterator = ll_iter_head(contents.functions);
    while(ll_iter_has_next(&iterator)) {
        func_declaration *dec = ll_iter_next(&iterator);
        symbol_table *symbols = st_sub_scope(funcReturns);
        linked_iter params = ll_iter_head(dec->parameters);
        while(ll_iter_has_next(&params)) {
            statement *param = ll_iter_next(&params);
            statement *param_type = ll_get_first(param->children);
            slice type_name = param_type->data;
            type type_val = get_type(contents, type_name);
            param_type->data = type_to_string(type_val);
            type *boxed_type = new(boxed_type);
            *boxed_type = type_val;
            st_put(symbols, &(param->data), boxed_type);
        }
        analyze_node(symbols, dec->root);
    }
}

static void analyze_node(symbol_table *context, statement *node) {
    switch(node->type) {
    case OP_INIT: {
        statement *name = ll_get_first(node->children);
        statement *value = ll_get_last(node->children);
        type t = get_node_type(context, value);
        type *boxed_type = new(boxed_type);
        *boxed_type = t;
        st_put(context, &(name->data), boxed_type);
        name->children = ll_new();
        statement *type_statement = new(type_statement);
        type_statement->children = NULL;
        type_statement->origin = name->origin;
        type_statement->type = TYPE;
        type_statement->data = type_to_string(t);
        ll_add_last(name->children, type_statement);
    } break;
    default:
        if(node->children != NULL) {
            linked_iter children = ll_iter_head(node->children);
            while(ll_iter_has_next(&children)) {
                statement *child = ll_iter_next(&children);
                analyze_node(context, child);
            }
        }
        break;
    }
}

static type get_node_type(symbol_table *context, statement *expr) {
    switch(expr->type) {
    case NAME:
        return *st_get_type(context, &(expr->data));
    case NUM:
        if(slice_contains(expr->data, '.'))
            return make_numeric_type(FLOAT, 32);
        else
            return make_numeric_type(SIGNED, 32);
    case OP_ASSIGN:
        return rightmost_type(context, expr);
    case OP_BOOL_OR:
    case OP_BOOL_XOR:
    case OP_BOOL_AND:
    case OP_EQUAL:
    case OP_NOT_EQUAL:
    case OP_GREATER:
    case OP_LESS:
    case OP_GREATER_EQUAL:
    case OP_LESS_EQUAL:
        return make_numeric_type(SIGNED, 8);
    case OP_DEREF:
       return dereference(rightmost_type(context, expr));
    case OP_GETREF:
       return reference(rightmost_type(context, expr));
    case OP_INDEX:
       return index(leftmost_type(context, expr));
    case FUNC_CALL:
       return *st_get_type(context, &(expr->data));
    default: {
        type t;
        linked_iter iterator = ll_iter_head(expr->children);
        statement *child = ll_iter_next(&iterator);
        t = get_node_type(context, child);
        while(ll_iter_has_next(&iterator)) {
            child = ll_iter_next(&iterator);
            type child_type = get_node_type(context, child);
            if(!type_equals(t, child_type)) {
                semantic_error("Mismatched types", expr->origin);
            }
        }
        return t;
    }
    }
}

static type leftmost_type(symbol_table *context, statement *expr) {
    statement *left = ll_get_first(expr->children);
    return get_node_type(context, left);
}


static type rightmost_type(symbol_table *context, statement *expr) {
    statement *right = ll_get_last(expr->children);
    return get_node_type(context, right);
}

