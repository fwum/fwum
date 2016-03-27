#include "tokenizer.h"
#include "slice.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

static parse_token *new_token(slice data, token_type type, char *filename, int line);
static void token_add(token_list *list, parse_token *token);
static bool is_whitespace(char c);
static bool is_alpha(char c);
static bool is_num(char c);
static void tokenizer_error(char *error, char *file, int line);
/*
SYMBOL, WORD, NUMBER, START, END, STRING_LIT, CHAR_LIT
*/
token_list parse(char *data, char *filename)
{
    token_list list = {NULL, NULL}; //Initialize the token list
    int length = strlen(data);
    int token_begin = 0;
    int source_line = 1; //keep track of the source line for error messages later
    enum {M_NONE, M_WORD, M_NUM, M_STRING, M_CHAR, M_COMMENT_LINE, M_COMMENT_MULTI} parse_mode;
    parse_mode = M_NONE;
    for(int i = 0; i < length; i++)
    {
        //Ingore carriage return characters so that windows newlines are identical to UNIX newlines
        if(data[i] == '\r') continue;
        if(data[i] == '\n') source_line += 1;
        //Handle comment parsing- comments are currently discarded
        if(data[i] == '/' && parse_mode != M_STRING && parse_mode != M_CHAR)
        {
            if(data[i + 1] == '*')
            {
                parse_mode = M_COMMENT_MULTI;
                continue;
            }
            else if(data[i + 1] == '/')
            {
                parse_mode = M_COMMENT_LINE;
                continue;
            }
        }
        //Parse the current character depending on context
        switch(parse_mode)
        {
        //Ignore all characters until a newline is reached
        case M_COMMENT_LINE:
            if(data[i] == '\n')
                parse_mode = M_NONE;
            break;
        //Ignore all characters until the end of the comment block is reached
        case M_COMMENT_MULTI:
            if(data[i] == '/' && data[i - 1] == '*')
                parse_mode = M_NONE;
            break;
        case M_NONE:
            if(is_whitespace(data[i])) continue;
            if(is_alpha(data[i]) || data[i] == '_')
            {
                parse_mode = M_WORD;
                token_begin = i;
            }
            else if(is_num(data[i]))
            {
                parse_mode = M_NUM;
                token_begin = i;
            }
            else if(data[i] == '\"')
            {
                parse_mode = M_STRING;
                token_begin = i + 1;
            }
            else if(data[i] == '\'')
            {
                parse_mode = M_CHAR;
                token_begin = i + 1;
            }
            else
            {
                token_add(&list, new_token(make_slice(&data[i], 1), SYMBOL, filename, source_line));
            }
            break;
        case M_WORD:
            if(is_whitespace(data[i]))
            {
                int length = i - token_begin;
                token_add(&list, new_token(make_slice(&data[token_begin], length), WORD, filename, source_line));
                parse_mode = M_NONE;
            }
            else if(!(is_alpha(data[i]) || is_num(data[i]) || data[i] == '_'))
            {
                int length = i - token_begin;
                token_add(&list, new_token(make_slice(&data[token_begin], length), WORD, filename, source_line));
                token_add(&list, new_token(make_slice(&data[i], 1), SYMBOL, filename, source_line));
                parse_mode = M_NONE;
            }
            else if(data[i] == '\"')
            {
                parse_mode = M_STRING;
                token_begin = i + 1;
            }
            else if(data[i] == '\'')
            {
                parse_mode = M_CHAR;
                token_begin = i + 1;
            }
            else if(i == length - 1)
            {
                token_add(&list, new_token(make_slice(&data[token_begin], length), WORD, filename, source_line));
                parse_mode = M_NONE;
            }
            break;
        case M_NUM:
            if(is_whitespace(data[i]))
            {
                int length = i - token_begin;
                token_add(&list, new_token(make_slice(&data[token_begin], length), NUMBER, filename, source_line));
                parse_mode = M_NONE;
            }
            else if(!(is_num(data[i]) || data[i] == '.'))
            {
                int length = i - token_begin;
                token_add(&list, new_token(make_slice(&data[token_begin], length), NUMBER, filename, source_line));
                token_add(&list, new_token(make_slice(&data[i], 1), SYMBOL, filename, source_line));
                parse_mode = M_NONE;
            }
            else if(data[i] == '\"')
            {
                parse_mode = M_STRING;
                token_begin = i + 1;
            }
            else if(data[i] == '\'')
            {
                parse_mode = M_CHAR;
                token_begin = i + 1;
            }
            else if(i == length - 1)
            {
                token_add(&list, new_token(make_slice(&data[token_begin], length), NUMBER, filename, source_line));
                parse_mode = M_NONE;
            }
            break;
        case M_STRING:
            if(data[i] == '\n')
            {
                tokenizer_error("Encountered newline while parsing string literal.", filename, source_line);
            }
            else if(data[i] == '\"' && data[i - 1] != '\\')
            {
                int length = i - token_begin;
                token_add(&list, new_token(make_slice(&data[token_begin], length), STRING_LIT, filename, source_line));
                parse_mode = M_NONE;
            }
            else if(i == length - 1)
            {
                tokenizer_error("Unexpected end of file while parsing string literal", filename, source_line);
            }
            break;
        case M_CHAR:
            if(data[i] == '\'')
            {
                int length = i - token_begin;
                token_add(&list, new_token(make_slice(&data[token_begin], length), CHAR_LIT, filename, source_line));
                parse_mode = M_NONE;
            }
            else if(i == length - 1)
            {
                tokenizer_error("Unexpected end of file while parsing character literal", filename, source_line);
            }
            else if(data[i + 1] != '\'' && data[i] != '\\')
            {
                tokenizer_error("Too many characters in a character literal", filename, source_line);
            }
            break;
        }
    }
    return list;
}

static void tokenizer_error(char *error, char *file, int line)
{
    fprintf(stderr, "Error encountered while parsing %s at line %d:\n%s\n", file, line, error);
    exit(-1);
}

void print_tlist(token_list list)
{
    parse_token *current = list.head;
    while(current != NULL)
    {
        switch(current->type)
        {
        case SYMBOL:
            printf("SYMBOL: %s\n", evaluate(current->data));
            break;
         case WORD:
            printf("WORD: %s\n", evaluate(current->data));
            break;
        case NUMBER:
            printf("NUMBER: %s\n", evaluate(current->data));
            break;
        case STRING_LIT:
            printf("STRING: %s\n", evaluate(current->data));
            break;
        case CHAR_LIT:
            printf("CHAR: %s\n", evaluate(current->data));
            break;
        }
        current = current->next;
    }
}

static parse_token *new_token(slice data, token_type type, char *filename, int line)
{
    parse_token *token = new(token);
    token->data = data;
    token->type = type;
    source_origin origin = {filename, line};
    token->origin = origin;
    token->next = NULL;
    return token;
}

static void token_add(token_list *list, parse_token *token)
{
    if(list->head == NULL)
    {
        list->head = token;
    }
    else if(list->tail == NULL)
    {
        list->head->next = token;
        list->tail = token;
    }
    else
    {
        list->tail->next = token;
        list->tail = token;
    }
}

static bool is_whitespace(char c)
{
    return c == ' ' || c == '\t' || c == '\n';
}

static bool is_alpha(char c)
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

static bool is_num(char c)
{
    return c >= '0' && c <= '9';
}
