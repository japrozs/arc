#include "errors.h"

void print_spaces(int num)
{
	for (int i = 0; i < num; i++)
		printf("%c", ' ');
}

void error_at_current(parser_t *parser, const char *msg, char *help_msg)
{

	// int i = 0;
	int line = parser->current->line;
	// printf("source :: %s\n", parser->lexer->source)
	// char *tok = strtok(parser->lexer->source, "\n");
	// while (tok != NULL)
	// {
	// 	tok = strtok(NULL, "\n");
	// 	printf("line :: %s\n", tok);
	// 	printf("i :: %d\n", i);
	// 	printf("line_no :: %d\n", line);
	// 	i++;
	// }
	// printf("line :: %s\n", tok);
	FILE *fp = fopen(parser->lexer->file_name, "r");
	char tok[8192];
	int i = 0;
	while (fgets(tok, sizeof(tok), fp) != NULL)
	{
		if (i == line)
		{
			break;
		}
	}

	printf("%serror%s: %s\n", RED_BOLD, RESET, msg);
	printf("%s--> %s:%d:%d%s\n", GRAY, parser->lexer->file_name, parser->current->line, (parser->lexer->offset - 1), RESET);
	printf("   |\n");
	printf("%*.d| %s\n", 3, parser->current->line, tok);
	printf("   |");
	print_spaces(parser->lexer->offset - 1);
	if (strcmp(help_msg, "") == 0)
	{
		printf("^ %serror%s\n", RED_BOLD, RESET);
	}
	else
	{
		printf("^ %shelp%s: %s\n", BLUE_BOLD, RESET, help_msg);
	}

	exit(EXIT_SUCCESS);
}
