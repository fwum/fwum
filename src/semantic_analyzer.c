#include <math.h>
#include "semantic_analyzer.h"
#include "parser.h"
#include "symbol_table.h"
#include "types.h"
#include "util.h"
#include "linked_list.h"

static type get_node_type(symbol_table *context, statement *expr);
static void analyze_node(symbol_table *context, statement *node);
static type leftmost_type(symbol_table *context, statement *expr);
static type rightmost_type(symbol_table *context, statement *expr);

void analyze(file_contents contents) {
    symbol_table *toplevelSymbols = st_new();
    //Analyze function return types
    linked_iter iterator = ll_iter_head(contents.functions);
    while(ll_iter_has_next(&iterator)) {
        func_declaration *dec = ll_iter_next(&iterator);
        type returnType = get_type(toplevelSymbols, dec->type);
        type *boxedType = new(boxedType);
        *boxedType = returnType;
        slice *boxedSlice = new(boxedSlice);
        *boxedSlice = dec->name;
        st_put(toplevelSymbols, boxedSlice, boxedType);
        dec->type = type_to_string(returnType);
    }
    //analyze struct and struct members
    iterator = ll_iter_head(contents.structs);
    while(ll_iter_has_next(&iterator)) {
        struct_declaration *dec = ll_iter_next(&iterator);
        type structtype;
        structtype.kind = STRUCT;
        structtype.data.declared = dec;
        type *boxedtype = new(boxedtype);
        *boxedtype = structtype;
        slice *boxedslice = new(boxedslice);
        *boxedslice = dec->name;
        st_put(toplevelSymbols, boxedslice, boxedtype);
    }
    //analyze unions and union members
    iterator = ll_iter_head(contents.unions);
    while(ll_iter_has_next(&iterator)) {
        struct_declaration *dec = ll_iter_next(&iterator);
        type structtype;
        structtype.kind = STRUCT;
        structtype.data.declared = dec;
        type *boxedtype = new(boxedtype);
        *boxedtype = structtype;
        slice *boxedslice = new(boxedslice);
        *boxedslice = dec->name;
        st_put(toplevelSymbols, boxedslice, boxedtype);
    }

    iterator = ll_iter_head(contents.enums);
    while(ll_iter_has_next(&iterator)) {
        enum_declaration *dec = ll_iter_next(&iterator);
        type enumType;
        enumType.kind = ENUM;
        enumType.data.enum_dec = dec;
        type *boxedType = new(boxedType);
        *boxedType = enumType;
        slice *boxedSlice = new(boxedSlice);
        *boxedSlice = dec->name;
        st_put(toplevelSymbols, boxedSlice, boxedType);
    }
    iterator = ll_iter_head(contents.functions);
    while(ll_iter_has_next(&iterator)) {
        func_declaration *dec = ll_iter_next(&iterator);
        symbol_table *symbols = st_sub_scope(toplevelSymbols);
        linked_iter params = ll_iter_head(dec->parameters);
        while(ll_iter_has_next(&params)) {
            statement *param = ll_iter_next(&params);
            statement *param_type = ll_get_first(param->children);
            slice type_name = param_type->data;
            type type_val = get_type(symbols, type_name);
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
        } break;
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
            return get_index(leftmost_type(context, expr));
        case FUNC_CALL:
            return *st_get_type(context, &(expr->data));
        case HEAP_INIT:
            return reference(*st_get_type(context, &((parse_token*)ll_get_first(expr->children))->data));
        case OP_MEMBER: {
            type left = leftmost_type(context, expr);
            statement *right = ll_get_last(expr->children);
            slice name = right->data;
            //TODO: Errors if the dot operator is invalid
            struct_declaration *dec = left.data.declared;
            linked_iter iterator = ll_iter_head(dec->members);
            while(ll_iter_has_next(&iterator)) {
                struct_member *member = ll_iter_next(&iterator);
                if(equals(member->name, name)) {
                    return get_type(context, member->type);
                }
            }
        }
        case OP_NAMESPACE: {
           type left = leftmost_type(context, expr);
           if(left.kind == ENUM) {
               int size = 8;
               int enum_items = ll_size(left.data.enum_dec->options);
               while(pow(2, size) < enum_items)
                   size *= 2;
               expr->type = NUM;
               statement *right = ll_get_last(expr->children);
               slice enum_item = right->data;
               int position = -1;
               linked_iter children = ll_iter_head(left.data.enum_dec->options);
               statement *child;
               do {
                   child = ll_iter_next(&children);
                   position++;
               } while (!equals(enum_item, child->data));
               //TODO: handle enum item not found
               expr->data = int_to_slice(position);
               return make_numeric_type(UNSIGNED, size);
           } else {
               //TODO: handle namespaces
               return left;
           }
        }

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

