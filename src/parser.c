#include "arc.h"
#include "errors.h"
#include "components/functions.h"
#include "components/variables.h"
#include "sym_table.h"
#include "parser.h"
#include "lexer.h"

void parser_advance(parser_t *parser)
{
	parser->previous = parser->current;

	for (;;)
	{
		parser->current = scan_token(parser->lexer);
		if (parser->current->type != TOKEN_ERROR)
			break;

		error_at_current(parser, parser->current->start, "");
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

void consume(parser_t *parser, token_type type, const char *message, char *help_msg)
{
	if (parser->current->type == type)
	{
		parser_advance(parser);
		return;
	}

	error_at_current(parser, message, help_msg);
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
	parser->current = malloc(sizeof(token_t));
	parser->previous = malloc(sizeof(token_t));

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
