#include "parser.h"
#include "slice.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
SYMBOL, WORD, NUMBER, START, END, STRING_LIT, CHAR_LIT
*/
token_list parse(char *data, char *filename)
{
    token_list list = {NULL, NULL};
    int length = strlen(data);
    int token_begin = 0, token_end = 0;
    int source_line = 0;
    token_type mode;
    for(int i = 0; i < length; i++)
    {
        //Ingore carriage return characters so that windows newlines are identical to UNIX newlines
        if(data[i] == '\r') continue;
        if(dtaa[i] == '\n') source_line += 1;

    }
    return list;
}

static parse_token *new_token(slice data, token_type type, char *filename, int line)
{
    parse_token *token = malloc(sizeof(*token));
    token->data = data;
    token->type = type;
    token->originFile = filename;
    token->line = line;
    token->next = NULL;
}

static void token_add(token_list list, parse_token token)
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
