#ifndef LEXER_H
#define LEXER_H

#include "arc.h"

typedef enum
{
	// Single-character tokens.
	TOKEN_LEFT_PAREN,
	TOKEN_RIGHT_PAREN,
	TOKEN_LEFT_BRACE,
	TOKEN_RIGHT_BRACE,
	TOKEN_COMMA,
	TOKEN_DOT,
	TOKEN_MINUS,
	TOKEN_PLUS,
	TOKEN_SLASH,
	TOKEN_COLON,
	TOKEN_SEMICOLON,
	TOKEN_STAR,
	// One or two character tokens.
	TOKEN_BANG,
	TOKEN_BANG_EQUAL,
	TOKEN_EQUAL,
	TOKEN_EQUAL_EQUAL,
	TOKEN_GREATER,
	TOKEN_GREATER_EQUAL,
	TOKEN_LESS,
	TOKEN_FLOAT,
	TOKEN_LESS_EQUAL,
	// Literals.
	TOKEN_IDENTIFIER,
	TOKEN_STRING,
	TOKEN_NUMBER,
	// Keywords.
	TOKEN_AND,
	TOKEN_CLASS,
	TOKEN_CONST,
	TOKEN_AT,
	TOKEN_ELSE,
	TOKEN_FALSE,
	TOKEN_FOR,
	TOKEN_FN,
	TOKEN_IF,
	TOKEN_NIL,
	TOKEN_OR,
	TOKEN_PRINT,
	TOKEN_RETURN,
	TOKEN_SUPER,
	TOKEN_THIS,
	TOKEN_TRUE,
	TOKEN_LET,
	TOKEN_WHILE,

	TOKEN_ERROR,
	TOKEN_EOF
} token_type;

typedef struct
{
	token_type type;
	char *start;
	int length;
	int line;
} token_t;

typedef struct
{
	char *start;
	char *current;
	char *source;
	char *file_name;
	FILE *file;
	int line;
	int offset;
} lexer_t;

lexer_t *init_lexer(char *source);
token_t *scan_token(lexer_t *lexer);

#endif // LEXER_H
