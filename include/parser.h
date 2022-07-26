#ifndef PARSER_H
#define PARSER_H

#include "arc.h"
#include "lexer.h"

typedef struct
{
	token_t *current;
	token_t *previous;
	lexer_t *lexer;
} parser_t;

parser_t *init_parser(lexer_t *lexer);
void parser_parse(parser_t *parser);
void consume(parser_t *parser, token_type type, const char *message, char *help_msg);
bool match(parser_t *parser, token_type type);

#endif // PARSER_H
