#include "lexer.h"

lexer_t *init_lexer(char *source)
{
	lexer_t *lexer = malloc(sizeof(lexer_t));
	lexer->start = malloc(sizeof(char) * strlen(source));
	strcpy(lexer->start, source);
	lexer->current = malloc(sizeof(char) * strlen(source));
	strcpy(lexer->current, source);
	lexer->line = 1;
	lexer->offset = 0;
	return lexer;
}

bool isAlpha(char c)
{
	return (c >= 'a' && c <= 'z') ||
		   (c >= 'A' && c <= 'Z') ||
		   c == '_';
}

bool isDigit(char c)
{
	return c >= '0' && c <= '9';
}

bool isAtEnd(lexer_t *lexer)
{
	return *lexer->current == '\0';
}

char advance(lexer_t *lexer)
{
	lexer->current++;
	lexer->offset++;
	return lexer->current[-1];
}

char peek(lexer_t *lexer)
{
	return *lexer->current;
}

char peekNext(lexer_t *lexer)
{
	if (isAtEnd(lexer))
		return '\0';
	return lexer->current[1];
}

bool lexer_match(lexer_t *lexer, char expected)
{
	if (isAtEnd(lexer))
		return false;
	if (*lexer->current != expected)
		return false;
	lexer->current++;
	return true;
}

token_t *makeToken(lexer_t *lexer, token_type type)
{
	token_t *token = malloc(sizeof(token_t));
	token->type = type;
	token->start = malloc(sizeof(char) * (lexer->current - lexer->start));
	strcpy(token->start, lexer->start);
	token->length = (int)(lexer->current - lexer->start);
	token->line = lexer->line;
	return token;
}

token_t *errorToken(lexer_t *lexer, const char *message)
{
	token_t *token = malloc(sizeof(token_t));
	token->type = TOKEN_ERROR;
	token->start = malloc(sizeof(char) * strlen(message));
	strcpy(token->start, message);
	token->length = (int)strlen(message);
	token->line = lexer->line;
	return token;
}

void skipWhitespace(lexer_t *lexer)
{
	for (;;)
	{
		char c = peek(lexer);
		switch (c)
		{
		case ' ':
		case '\r':
		case '\t':
			advance(lexer);
			break;

		case '\n':
			lexer->line++;
			lexer->offset = 0;
			advance(lexer);
			break;

		case '/':
			if (peekNext(lexer) == '/')
			{

				while (peek(lexer) != '\n' && !isAtEnd(lexer))
					advance(lexer);
			}
			else
			{
				return;
			}
			break;

		default:
			return;
		}
	}
}

token_type checkKeyword(lexer_t *lexer, int start, int length,
						const char *rest, token_type type)
{
	if (lexer->current - lexer->start == start + length &&
		memcmp(lexer->start + start, rest, length) == 0)
	{
		return type;
	}

	return TOKEN_IDENTIFIER;
}

token_type identifierType(lexer_t *lexer)
{

	switch (lexer->start[0])
	{
	case 'a':
		return checkKeyword(lexer, 1, 2, "nd", TOKEN_AND);
	case 'c':
		if (lexer->current - lexer->start > 1)
		{
			switch (lexer->start[1])
			{
			case 'o':
				return checkKeyword(lexer, 2, 3, "nst", TOKEN_CONST);
			case 'l':
				return checkKeyword(lexer, 2, 3, "ass", TOKEN_CLASS);
			}
		}
		break;
	case 'e':
		return checkKeyword(lexer, 1, 3, "lse", TOKEN_ELSE);

	case 'f':
		if (lexer->current - lexer->start > 1)
		{
			switch (lexer->start[1])
			{
			case 'a':
				return checkKeyword(lexer, 2, 3, "lse", TOKEN_FALSE);
			case 'n':
				return TOKEN_FN;
			case 'o':
				return checkKeyword(lexer, 2, 1, "r", TOKEN_FOR);
			}
		}
		break;

	case 'i':
		return checkKeyword(lexer, 1, 1, "f", TOKEN_IF);
	case 'l':
		return checkKeyword(lexer, 1, 2, "et", TOKEN_LET);
	case 'n':
		return checkKeyword(lexer, 1, 2, "il", TOKEN_NIL);
	case 'o':
		return checkKeyword(lexer, 1, 1, "r", TOKEN_OR);
	case 'r':
		return checkKeyword(lexer, 1, 5, "eturn", TOKEN_RETURN);
	case 's':
		return checkKeyword(lexer, 1, 4, "uper", TOKEN_SUPER);

	case 't':
		if (lexer->current - lexer->start > 1)
		{
			switch (lexer->start[1])
			{
			case 'h':
				return checkKeyword(lexer, 2, 2, "is", TOKEN_THIS);
			case 'r':
				return checkKeyword(lexer, 2, 2, "ue", TOKEN_TRUE);
			}
		}
		break;

	case 'w':
		return checkKeyword(lexer, 1, 4, "hile", TOKEN_WHILE);
	}

	return TOKEN_IDENTIFIER;
}

token_t *identifier(lexer_t *lexer)
{
	while (isAlpha(peek(lexer)) || isDigit(peek(lexer)))
		advance(lexer);
	return makeToken(lexer, identifierType(lexer));
}

token_t *number(lexer_t *lexer)
{
	while (isDigit(peek(lexer)))
		advance(lexer);

	if (peek(lexer) == '.' && isDigit(peekNext(lexer)))
	{

		advance(lexer);

		while (isDigit(peek(lexer)))
			advance(lexer);

		return makeToken(lexer, TOKEN_FLOAT);
	}

	return makeToken(lexer, TOKEN_NUMBER);
}

token_t *string(lexer_t *lexer, char quote_type)
{
	while (peek(lexer) != quote_type && !isAtEnd(lexer))
	{
		if (peek(lexer) == '\n')
			lexer->line++;
		advance(lexer);
	}

	if (isAtEnd(lexer))
		return errorToken(lexer, "Unterminated string.");

	advance(lexer);
	return makeToken(lexer, TOKEN_STRING);
}

token_t *scan_token(lexer_t *lexer)
{

	skipWhitespace(lexer);

	lexer->start = lexer->current;

	if (isAtEnd(lexer))
		return makeToken(lexer, TOKEN_EOF);

	char c = advance(lexer);

	if (isAlpha(c))
		return identifier(lexer);

	if (isDigit(c))
		return number(lexer);

	switch (c)
	{
	case '(':
		return makeToken(lexer, TOKEN_LEFT_PAREN);
	case ')':
		return makeToken(lexer, TOKEN_RIGHT_PAREN);
	case '@':
		return makeToken(lexer, TOKEN_AT);
	case '{':
		return makeToken(lexer, TOKEN_LEFT_BRACE);
	case '}':
		return makeToken(lexer, TOKEN_RIGHT_BRACE);
	case ';':
		return makeToken(lexer, TOKEN_SEMICOLON);
	case ':':
		return makeToken(lexer, TOKEN_COLON);
	case ',':
		return makeToken(lexer, TOKEN_COMMA);
	case '.':
		return makeToken(lexer, TOKEN_DOT);
	case '-':
		return makeToken(lexer, TOKEN_MINUS);
	case '+':
		return makeToken(lexer, TOKEN_PLUS);
	case '/':
		return makeToken(lexer, TOKEN_SLASH);
	case '*':
		return makeToken(lexer, TOKEN_STAR);

	case '!':
		return makeToken(lexer,
						 lexer_match(lexer, '=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
	case '=':
		return makeToken(lexer,
						 lexer_match(lexer, '=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
	case '<':
		return makeToken(lexer,
						 lexer_match(lexer, '=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
	case '>':
		return makeToken(lexer,
						 lexer_match(lexer, '=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);

	case '"':
		return string(lexer, '"');
	case '\'':
		return string(lexer, '\'');
	}

	return errorToken(lexer, "Unexpected character.");
}
