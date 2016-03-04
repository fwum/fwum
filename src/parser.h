#ifndef PARSER_H_
#define PARSER_H_
#include "slice.h"
struct parse_token;
struct token_list;
typedef struct parse_token parse_token;
typedef struct token_list token_list;
struct parse_token {
    slice data;
    enum {OPERATOR, WORD, NUMBER, BLOCK_START, BLOCK_END, STRING, CHARACTER} type;
    char *originFile;
    int line;
    parse_token *next;
};
struct token_list {
    parse_token *head, *tail;
};
parse_token *parse(char *data, char *filename);
#endif
