#ifndef PARSER_H_
#define PARSER_H_
#include "slice.h"
#include "util.h"
#include "linked_list.h"

DEFSTRUCT(parse_token);
DEFSTRUCT(token_list);
DEFSTRUCT(source_origin);
DEFSTRUCT(parse_source);

struct source_origin {
    char *filename;
    int line;
};
typedef enum token_type {
    SYMBOL, WORD, NUMBER, STRING_LIT, CHAR_LIT
} token_type;
/*
A token from a source filename
The result from the direct-text read and the operand for all future steps in compilation
*/
struct parse_token {
    slice data;
    token_type type;
    source_origin origin;
};
/*
The source of a parse
It contains the state of a parse on a file
*/
struct parse_source {
    char *filename;
    char *data;
    int line, pos, length;
};
/*
Get a struct that holds the state of a parse on a file
*/
parse_source start_parse(char *data, char *filename);
/*
Get a token from a file
*/
parse_token get_token(parse_source *source);
/*
Tells if there is another token left in the source
*/
bool has_token(parse_source source);
#endif
