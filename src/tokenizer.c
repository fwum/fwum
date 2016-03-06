#include "parser.h"
#include <stdlib.h>
#include <string.h>

/*
SYMBOL, WORD, NUMBER, START, END, STRING_LIT, CHAR_LIT
*/
parse_token *parse(char *data, char *filename)
{
    token_list list = {NULL, NULL};
    slice current = make_slice(data, 0);
    int length = strlen(data);
    for(int i = 0; i < length; i++)
    {

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
