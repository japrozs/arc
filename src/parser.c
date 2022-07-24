#include "arc.h"
#include "components/functions.h"
#include "components/variables.h"
#include "parser.h"
#include "lexer.h"

void error_at_current(parser_t *parser, const char *msg)
{
	if (parser->panic_mode)
		return;
	fprintf(stderr, "[line %d] Error", parser->current->line);

	if (parser->current->type == TOKEN_EOF)
	{
		fprintf(stderr, " at end");
	}
	else
	{
		fprintf(stderr, " at '%.*s'", parser->current->length, parser->current->start);
	}

	fprintf(stderr, ": %s\n", msg);
	parser->had_error = true;
	exit(EXIT_SUCCESS);
}

void parser_advance(parser_t *parser)
{
	parser->previous = parser->current;

	for (;;)
	{
		parser->current = scan_token(parser->lexer);
		if (parser->current->type != TOKEN_ERROR)
			break;

		error_at_current(parser, parser->current->start);
	}
}

bool check(parser_t *parser, token_type type)
{
	return parser->current->type == type;
}

bool match(parser_t *parser, token_type type)
{
	if (!check(parser, type))
		return false;
	parser_advance(parser);
	return true;
}

void consume(parser_t *parser, token_type type, const char *message)
{
	if (parser->current->type == type)
	{
		parser_advance(parser);
		return;
	}

	error_at_current(parser, message);
}

void declaration(parser_t *parser)
{
	if (match(parser, TOKEN_LET))
	{
		build_var(parser);
	}
	// else if (match(TOKEN_FN))
	// {
	// 	build_fn(module, builder, context);
	// }
	// else if (match(TOKEN_IDENTIFIER))
	// {
	// 	char *var = malloc(sizeof(char) * parser.previous.length);
	// 	sprintf(var, "%.*s", parser.previous.length, parser.previous.start);
	// 	if (match(TOKEN_LEFT_PAREN))
	// 	{
	// 		build_fn_call(module, builder, context, var);
	// 	}
	// }
}

parser_t *init_parser(lexer_t *lexer)
{
	parser_t *parser = malloc(sizeof(parser_t));
	parser->lexer = lexer;
	parser->had_error = false;
	parser->current = malloc(sizeof(token_t));
	parser->previous = malloc(sizeof(token_t));
	parser->panic_mode = false;

	return parser;
}

void parser_parse(parser_t *parser)
{
	parser_advance(parser);
	while (!match(parser, TOKEN_EOF))
	{
		declaration(parser);
	}
}
