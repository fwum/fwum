#ifndef PARSER_H_
#define PARSER_H_
#include "slice.h"
#include "util.h"
DEFSTRUCT(parse_token);
DEFSTRUCT(token_list);
DEFSTRUCT(source_origin);
struct source_origin {
    char *filename;
    int line;
};
typedef enum token_type {
    SYMBOL, WORD, NUMBER, STRING_LIT, CHAR_LIT
} token_type;
struct parse_token {
    slice data;
    token_type type;
    source_origin origin;
    parse_token *next;
};
struct token_list {
    parse_token *head, *tail;
};

token_list parse(char *data, char *filename);
void print_tlist(token_list list);
#endif
