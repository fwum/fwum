#include "tokenizer.h"
#include "slice.h"
#include "linked_list.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

static parse_token new_token(slice data, token_type type, char *filename, int line);
static bool is_whitespace(char c);
static bool is_alpha(char c);
static bool is_num(char c);
static void tokenizer_error(char *error, char *file, int line);
static int next_significant_char(parse_source source, int *newlineAccumulator);

parse_source start_parse(char *data, char *filename) {
    parse_source source;
    source.data = data;
    source.filename = filename;
    source.line = 1;
    source.pos = 0;
    source.length = strlen(data);
    return source;
}

bool has_token(parse_source source) {
    int newlineThrowaway = 0; //Pass to the function to just throw away the newlines it finds
    return next_significant_char(source, &newlineThrowaway) != -1;
}

parse_token get_token(parse_source *source) {
    enum {M_NONE, M_WORD, M_NUM, M_STRING, M_CHAR, M_COMMENT_LINE, M_COMMENT_MULTI} parse_mode;
    parse_mode = M_NONE;
    int token_begin = source->pos;
    for(int i = source->pos; i < source->length; i++) {
        if(source->data[i] == '\r') {
            continue;
        } else if(source->data[i] == '\n') {
            source->line += 1;
        } else if(source->data[i] && parse_mode != M_STRING && parse_mode != M_CHAR) {
            if(source->data[i + 1] == '*') {
                parse_mode = M_COMMENT_MULTI;
                continue;
            } else if(source->data[i + 1] == '/') {
                parse_mode = M_COMMENT_LINE;
                continue;
            }
        }
        //Parse the current character depending on context
        switch(parse_mode) {
        //Ignore all characters until a newline is reached
        case M_COMMENT_LINE:
            if(source->data[i] == '\n')
                parse_mode = M_NONE;
            break;
        //Ignore all characters until the end of the comment block is reached
        case M_COMMENT_MULTI:
            if(source->data[i] == '/' && source->data[i - 1] == '*')
                parse_mode = M_NONE;
            else if(i == source->length - 1)
                tokenizer_error("Unexpected end of file reached during multi-line comment", source->filename, source->line);
            break;
        //Continue until we begin to encounter useful character
        case M_NONE:
            //Ignore extra whitespace
            if(is_whitespace(source->data[i])) {
                continue;
            }
            //Beginning of a valid identifier
            if(is_alpha(source->data[i]) || source->data[i] == '_') {
                parse_mode = M_WORD;
                token_begin = i;
            } else if(is_num(source->data[i])) {
                parse_mode = M_NUM;
                token_begin = i;
            } else if(source->data[i] == '\"') {
                parse_mode = M_STRING;
                token_begin = i + 1;
            } else if(source->data[i] == '\'') {
                parse_mode = M_CHAR;
                token_begin = i + 1;
            } else {
                source->pos = i + 1;
                return new_token(make_slice(&(source->data[i]), 1), SYMBOL, source->filename, source->line);
            }
            break;
        //Continue until the end of the word is reached, then add it as a token
        case M_WORD:
            if(is_whitespace(source->data[i]) || i == source->length - 1 || !(is_alpha(source->data[i]) || is_num(source->data[i]) || source->data[i] == '_')) {
                int length = i - token_begin;
                source->pos = i;
                return new_token(make_slice(&(source->data[token_begin]), length), WORD, source->filename, source->line);
            }
            break;
        //Continue until the end of the number is reached, then add it as a token
        case M_NUM:
            if(is_whitespace(source->data[i]) || !(is_num(source->data[i]) || source->data[i] == '.') || i == source->length - 1) {
                int length = i - token_begin;
                source->pos = i;
                return new_token(make_slice(&(source->data[token_begin]), length), NUMBER, source->filename, source->line);
            }
            break;
        //Continue until the end of the string literal is reached, then add it as a token
        case M_STRING:
            if(source->data[i] == '\n') {
                tokenizer_error("Encountered newline while parsing string literal.", source->filename, source->line);
            } else if(source->data[i] == '\"' && source->data[i - 1] != '\\') {
                int length = i - token_begin;
                source->pos = i + 1;
                return new_token(make_slice(&(source->data[token_begin]), length), STRING_LIT, source->filename, source->line);
            } else if(i == source->length - 1) {
                tokenizer_error("Unexpected end of file while parsing string literal", source->filename, source->line);
            }
            break;
        //Parse a character literal
        case M_CHAR:
            if(source->data[i] == '\'') {
                int length = i - token_begin;
                source->pos = i + 1;
                return new_token(make_slice(&(source->data[token_begin]), length), CHAR_LIT, source->filename, source->line);
            } else if(i == source->length - 1) {
                tokenizer_error("Unexpected end of file while parsing character literal", source->filename, source->line);
            } else if(source->data[i + 1] != '\'' && source->data[i] != '\\') {
                tokenizer_error("Too many characters in a character literal", source->filename, source->line);
            }
            break;
        }
    }
    fprintf(stderr, "Call for token when context is empty");
    exit(-1);
}

static int next_significant_char(parse_source source, int *newlines) {
    enum {SINGLE_LINE_COMMENT, MULTI_LINE_COMMENT, REGULAR_TEXT} mode;
    mode = REGULAR_TEXT;
    for(int i = source.pos; i < source.length; i++) {
        if(source.data[i] == '\n')
            *newlines += 1;
        switch(mode) {
        case REGULAR_TEXT:
            if(source.data[i] == '/' && i < source.length - 1) {
                if(source.data[i + 1] == '/') {
                    mode = SINGLE_LINE_COMMENT;
                } else if(source.data[i + 1] == '*') {
                    mode = MULTI_LINE_COMMENT;
                }
            } else if(source.data[i] != ' ' && source.data[i] != '\r')
                return i;
        case SINGLE_LINE_COMMENT:
            if(source.data[i] == '\n')
                mode = REGULAR_TEXT;
            break;
        case MULTI_LINE_COMMENT:
            if(source.data[i] == '/' && source.data[i - 1] == '*')
                mode = REGULAR_TEXT;
            break;
        }
    }
    return -1;
}

static void tokenizer_error(char *error, char *file, int line) {
    fprintf(stderr, "Error encountered while parsing %s at line %d:\n%s\n", file, line, error);
    exit(-1);
}

static parse_token new_token(slice data, token_type type, char *filename, int line) {
    parse_token token;
    token.data = data;
    token.type = type;
    source_origin origin = {filename, line};
    token.origin = origin;
    return token;
}

static bool is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n';
}

static bool is_alpha(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

static bool is_num(char c) {
    return c >= '0' && c <= '9';
}
