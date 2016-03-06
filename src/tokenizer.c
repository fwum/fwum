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

/*
SYMBOL, WORD, NUMBER, START, END, STRING_LIT, CHAR_LIT
*/
token_list parse(char *data, char *filename)
{
    token_list list = {NULL, NULL};
    int length = strlen(data);
    int token_begin = 0;
    int source_line = 0;
    enum {M_NONE, M_WORD, M_NUM, M_STRING, M_CHAR} parse_mode;
    parse_mode = M_NONE;
    for(int i = 0; i < length; i++)
    {
        //Ingore carriage return characters so that windows newlines are identical to UNIX newlines
        if(data[i] == '\r') continue;
        if(data[i] == '\n') source_line += 1;
        switch(parse_mode)
        {
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
            break;
        case M_STRING:
            break;
        case M_CHAR:
            break;
        }
    }
    return list;
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
    parse_token *token = malloc(sizeof(*token));
    token->data = data;
    token->type = type;
    token->originFile = filename;
    token->line = line;
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
