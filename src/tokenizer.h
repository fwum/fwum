#ifndef PARSER_H_
#define PARSER_H_
#include "slice.h"
struct parse_token;
struct token_list;
enum token_type;
typedef struct parse_token parse_token;
typedef struct token_list token_list;
typedef enum token_type token_type;
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
enum token_type {
    OPERATOR, WORD, NUMBER, START, END, STRING_LIT, CHAR_LIT;
}
parse_token *parse(char *data, char *filename);
#endif
