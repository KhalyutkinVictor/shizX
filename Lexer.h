//
// Created by victor on 04.05.23.
//

#ifndef SHIZX_LEXER_H
#define SHIZX_LEXER_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Mvec.h"

const char *KEYWORDS[] = {
    "let",
    "as"
};
const size_t KEYWORDS_LEN = 2;

const char *OPS[] = {
    "=",
    "+",
    "-",
    "*",
    "/",
    "&&",
    "||",
    ">",
    "<",
    ">=",
    "<=",
    "==",
    "!="
};
const size_t OPS_LENGTH = 13;

const char *PUNCTS[] = {
    "(",
    ")",
    "{",
    "}",
    ",",
    ";",
    ".",
};
const size_t PUNCTS_LEN = 7;

typedef enum {
    KEYWORD,
    OP,
    PUNCT,
    NUM,
    STR,
    VAR
} LEXEME_TYPE;

typedef struct {
    char *start;
    size_t len;

    LEXEME_TYPE lexeme_type;
} Lexeme;

int is_char_in_strings(char c, size_t at_pos, const char *strs[], size_t len)
{
    for (int i = 0; i < len; i++) {
        if (strlen(strs[i]) > at_pos && strs[i][at_pos] == c) {
            return 1;
        }
    }
    return 0;
}

int ifispuncts(char c, size_t at_pos)
{
    return is_char_in_strings(c, at_pos, PUNCTS, PUNCTS_LEN);
}

int isops(char c, size_t at_pos)
{
    return is_char_in_strings(c, at_pos, OPS, OPS_LENGTH);
}

char *lex_to_str(Lexeme *l)
{
    char *str = memcpy(malloc((l->len + 1) * sizeof(char)), l->start, l->len);
    str[l->len] = '\0';
    return str;
}

void debug_lex(Lexeme *lex)
{
    char *type = "";
    switch (lex->lexeme_type) {
        case KEYWORD:
            type = "KEYWORD";
            break;
        case OP:
            type = "OP";
            break;
        case PUNCT:
            type = "PUNCT";
            break;
        case NUM:
            type = "NUM\0";
            break;
        case STR:
            type = "STR";
            break;
        case VAR:
            type = "VAR\0";
            break;
    }
    char *lexstr = lex_to_str(lex);
    printf("Lexeme { lex: \"%s\", type: %s }\n", lexstr, type);
    free(lexstr);
}

void debug_mvec_lex(Mvec *mvec)
{
    printf("Mvec<Lexeme*>{\n");
    for (int i = 0; i < mvec->len; i++) {
        Lexeme *lex = mvec_get_at(mvec, i);
        printf("\t");
        debug_lex(lex);
    }
    printf("}\n");
}

void throw_error(size_t pos, char *error_text)
{
    printf("Error at pos: %zu\n%s\n", pos, error_text);
    exit(23);
}

char *peek_next_char(char *code)
{
    return code;
}

void get_alnum_lex_type(Lexeme *lex)
{
    // TODO: free
    char *str = lex_to_str(lex);
    LEXEME_TYPE type = VAR;
    for (int i = 0; i < KEYWORDS_LEN; i++) {
        if (strcmp(str, KEYWORDS[i]) == 0) {
            type = KEYWORD;
        }
    }
    lex->lexeme_type = type;
    free(str);
}

Lexeme *parse_number(char *text, size_t len, size_t absolute_pos)
{
    int i = 0;
    while (isdigit(*(text + i)) && i < len) {
        i++;
    }
    Lexeme *lex = malloc(sizeof(Lexeme));
    lex->start = text;
    lex->len = i;
    lex->lexeme_type = NUM;
    return lex;
}

Lexeme *parse_alpha(char *text, size_t len, size_t absolute_pos)
{
    int i = 0;
    while (isalnum(*(text + i)) && i < len) {
        i++;
    }
    Lexeme *lex = malloc(sizeof(Lexeme));
    lex->start = text;
    lex->len = i;
    get_alnum_lex_type(lex);
    return lex;
}

Lexeme *parse_op(char *text, size_t len, size_t absolute_pos)
{
    int i = 0;
    while (isops(*(text + i), i) && i < len) {
        i++;
    }
    Lexeme *lex = malloc(sizeof(Lexeme));
    lex->start = text;
    lex->len = i;
    lex->lexeme_type = OP;
    return lex;
}

Lexeme *parse_punct(char *text, size_t len, size_t absolute_pos)
{
    int i = 0;
    while (ifispuncts(*(text + i), i) && i < len) {
        i++;
    }
    Lexeme *lex = malloc(sizeof(Lexeme));
    lex->start = text;
    lex->len = i;
    lex->lexeme_type = PUNCT;
    return lex;
}

// "qwer"
// 012345

Lexeme *parse_str(char *text, size_t len, size_t absolute_pos)
{
    int i = 0;
    int is_escaped = 1;
    while (!(*(text + i) == '"' && !is_escaped) && i < len) {
        if (*(text + i) == '\\') {
            is_escaped = 1;
        } else {
            is_escaped = 0;
        }
        i++;
    }
    if (i == len && *(text + len - 1) != '"') {
        throw_error(absolute_pos + len - 1, "Can not find string ending");
    }
    Lexeme *lex = malloc(sizeof(Lexeme));
    lex->start = text + 1;
    lex->len = i - 1;
    lex->lexeme_type = STR;
    return lex;
}

Mvec *tokenize(char *text, size_t len)
{
    char c;
    int i = 0;
    Mvec *lex_vec = mvec_create(sizeof(Lexeme));
    while (i < len) {
        c = *(text + i);
        if (isspace(c)) {
            i++;
            continue;
        }
        Lexeme *lex;
        if (isdigit(c)) {
            lex = parse_number(text + i, len - i, i);
            mvec_push(lex_vec, lex);
            i += lex->len;
            continue;
        }
        if (isalpha(c)) {
            lex = parse_alpha(text + i, len - i, i);
            mvec_push(lex_vec, lex);
            i += lex->len;
            continue;
        }
        if (isops(c, 0)) {
            lex = parse_op(text + i, len - i, i);
            mvec_push(lex_vec, lex);
            i += lex->len;
            continue;
        }
        if (ifispuncts(c, 0)) {
            lex = parse_punct(text + i, len - i, i);
            mvec_push(lex_vec, lex);
            i += lex->len;
            continue;
        }
        if (c == '"') {
            lex = parse_str(text + i, len - i, i);
            mvec_push(lex_vec, lex);
            i += lex->len + 2; // because in lexeme we strip '"' symbols from start and end
            continue;
        }
        i++;
    }
    return lex_vec;
}

#endif //SHIZX_LEXER_H
