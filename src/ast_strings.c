#include "ast_strings.h"
#include "parser.h"
#include <stdlib.h>

statement_type statement_from_slice(slice string) {
	if(equals_string(string, "OP_ADD")) {
		return OP_ADD;
	} else if(equals_string(string, "OP_SUB")) {
		return OP_SUB;
	} else if(equals_string(string, "OP_MULT")) {
		return OP_MULT;
	} else if(equals_string(string, "OP_DIV")) {
		return OP_DIV;
	} else if(equals_string(string, "OP_MOD")) {
		return OP_MOD;
	} else if(equals_string(string, "OP_EXP")) {
		return OP_EXP;
	} else if(equals_string(string, "OP_BIT_AND")) {
		return OP_BIT_AND;
	} else if(equals_string(string, "OP_BOOL_AND")) {
		return OP_BOOL_AND;
	} else if(equals_string(string, "OP_BIT_OR")) {
		return OP_BIT_OR;
	} else if(equals_string(string, "OP_BOOL_OR")) {
		return OP_BOOL_OR;
	} else if(equals_string(string, "OP_BIT_XOR")) {
		return OP_BIT_XOR;
	} else if(equals_string(string, "OP_BOOL_XOR")) {
		return OP_BOOL_XOR;
	} else if(equals_string(string, "OP_BIT_NOT")) {
		return OP_BIT_NOT;
	} else if(equals_string(string, "OP_BOOL_NOT")) {
		return OP_BOOL_NOT;
	} else if(equals_string(string, "OP_SHIFT_LEFT")) {
		return OP_SHIFT_LEFT;
	} else if(equals_string(string, "OP_SHIFT_RIGHT")) {
		return OP_SHIFT_RIGHT;
	} else if(equals_string(string, "OP_BIT_SHIFT_RIGHT")) {
		return OP_BIT_SHIFT_RIGHT;
	} else if(equals_string(string, "OP_GREATER")) {
		return OP_GREATER;
	} else if(equals_string(string, "OP_LESS")) {
		return OP_LESS;
	} else if(equals_string(string, "OP_GREATER_EQUAL")) {
		return OP_GREATER_EQUAL;
	} else if(equals_string(string, "OP_LESS_EQUAL")) {
		return OP_LESS_EQUAL;
	} else if(equals_string(string, "OP_EQUAL")) {
		return OP_EQUAL;
	} else if(equals_string(string, "OP_NOT_EQUAL")) {
		return OP_NOT_EQUAL;
	} else if(equals_string(string, "OP_ASSIGN")) {
		return OP_ASSIGN;
	} else if(equals_string(string, "OP_INIT")) {
		return OP_INIT;
	} else if(equals_string(string, "OP_MEMBER")) {
		return OP_MEMBER;
	} else if(equals_string(string, "OP_GETREF")) {
		return OP_GETREF;
	} else if(equals_string(string, "OP_DEREF")) {
		return OP_DEREF;
	} else if(equals_string(string, "OP_INDEX")) {
		return OP_INDEX;
	} else if(equals_string(string, "RETURN")) {
		return RETURN;
	} else if(equals_string(string, "CONTINUE")) {
		return CONTINUE;
	} else if(equals_string(string, "BREAK")) {
		return BREAK;
	} else if(equals_string(string, "FUNC_CALL")) {
		return FUNC_CALL;
	} else if(equals_string(string, "STACK_INIT")) {
		return STACK_INIT;
	} else if(equals_string(string, "HEAP_INIT")) {
		return HEAP_INIT;
	} else if(equals_string(string, "IF")) {
		return IF;
	} else if(equals_string(string, "ELSE")) {
		return ELSE;
	} else if(equals_string(string, "WHILE")) {
		return WHILE;
	} else if(equals_string(string, "FOR")) {
		return FOR;
	} else if(equals_string(string, "FOREACH")) {
		return FOREACH;
	} else if(equals_string(string, "BLOCK")) {
		return BLOCK;
	} else if(equals_string(string, "TYPE")) {
		return TYPE;
	} else if(equals_string(string, "NAME")) {
		return NAME;
	} else if(equals_string(string, "ROOT")) {
		return ROOT;
	} else if(equals_string(string, "STRING")) {
		return STRING;
	} else if(equals_string(string, "CHAR")) {
		return CHAR;
	} else if(equals_string(string, "NUM")) {
		return NUM;
	} else if(equals_string(string, "VALUE_TRUE")) {
		return VALUE_TRUE;
	} else if(equals_string(string, "VALUE_FALSE")) {
		return VALUE_FALSE;
	} else {
		exit(-1);
		return -1;
	}
}

char *statement_to_string(statement_type type) {
	switch(type) {
        case OP_ADD:
        	return "OP_ADD";
        break;
        case OP_SUB:
        	return "OP_SUB";
        break;
        case OP_MULT:
        	return "OP_MULT";
        break;
        case OP_DIV:
        	return "OP_DIV";
        break;
        case OP_MOD:
        	return "OP_MOD";
        break;
        case OP_EXP:
            return "OP_EXP";
        break;
        case OP_BIT_AND:
        	return "OP_BIT_AND";
        break;
        case OP_BOOL_AND:
        	return "OP_BOOL_AND";
        break;
        case OP_BIT_OR:
        	return "OP_BIT_OR";
        break;
        case OP_BOOL_OR:
        	return "OP_BOOL_OR";
        break;
        case OP_BIT_XOR:
        	return "OP_BIT_XOR";
        break;
        case OP_BOOL_XOR:
        	return "OP_BOOL_XOR";
        break;
        case OP_BIT_NOT:
        	return "OP_BIT_NOT";
        break;
        case OP_BOOL_NOT:
        	return "OP_BOOL_NOT";
        break;
        case OP_SHIFT_LEFT:
            return "OP_SHIFT_LEFT";
        break;
        case OP_SHIFT_RIGHT:
            return "OP_SHIFT_RIGHT";
        break;
        case OP_BIT_SHIFT_RIGHT:
            return "OP_BIT_SHIFT_RIGHT";
        break;
        case OP_GREATER:
        	return "OP_GREATER";
        break;
        case OP_LESS:
        	return "OP_LESS";
        break;
        case OP_GREATER_EQUAL:
        	return "OP_GREATER_EQUAL";
        break;
        case OP_LESS_EQUAL:
        	return "OP_LESS_EQUAL";
        break;
        case OP_EQUAL:
        	return "OP_EQUAL";
        break;
        case OP_NOT_EQUAL:
        	return "OP_NOT_EQUAL";
        break;
        case OP_ASSIGN:
        	return "OP_ASSIGN";
        break;
		case OP_INIT:
			return "OP_INIT";
		break;
        case OP_MEMBER:
        	return "OP_MEMBER";
        break;
        case OP_GETREF:
        	return "OP_GETREF";
        break;
        case OP_DEREF:
        	return "OP_DEREF";
        break;
        case OP_INDEX:
        	return "OP_INDEX";
        break;
        case RETURN:
        	return "RETURN";
        break;
        case CONTINUE:
        	return "CONTINUE";
        break;
        case BREAK:
            return "BREAK";
        break;
        case FUNC_CALL:
        	return "FUNC_CALL";
        break;
        case STACK_INIT:
        	return "STACK_INIT";
        break;
        case HEAP_INIT:
        	return "HEAP_INIT";
        break;
        case IF:
        	return "IF";
        break;
		case ELSE:
			return "ELSE";
		break;
        case WHILE:
        	return "WHILE";
        break;
        case FOR:
        	return "FOR";
        break;
        case FOREACH:
        	return "FOREACH";
        break;
        case BLOCK:
        	return "BLOCK";
        break;
        case TYPE:
        	return "TYPE";
        break;
        case NAME:
        	return "NAME";
        break;
        case ROOT:
        	return "ROOT";
        break;
        case STRING:
        	return "STRING";
        break;
        case CHAR:
        	return "CHAR";
        break;
        case NUM:
        	return "NUM";
        break;
        case VALUE_TRUE:
            return "TRUE";
        break;
        case VALUE_FALSE:
            return "FALSE";
        break;
		default:
			return "UNKNOWN_AST_TYPE";
		break;
	}
}
