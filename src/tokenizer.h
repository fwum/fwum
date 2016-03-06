#ifndef PARSER_H_
#define PARSER_H_
#include "slice.h"
struct parse_token;
struct token_list;
typedef struct parse_token parse_token;
typedef struct token_list token_list;
typedef enum token_type {
    SYMBOL, WORD, NUMBER, STRING_LIT, CHAR_LIT
} token_type;
struct parse_token {
    slice data;
    token_type type;
    char *originFile;
    int line;
    parse_token *next;
};
struct token_list {
    parse_token *head, *tail;
};
token_list parse(char *data, char *filename);
void print_tlist(token_list list);
#endif
