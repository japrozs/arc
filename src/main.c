#include "arc.h"
#include "lexer.h"
#include "sym_table.h"
#include "parser.h"
#include "llvm_cfg.h"

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("%serror%s: incorrect number of arguments\n", RED_BOLD, RESET);
		printf("\nusage:: vel <filename>\n");
		exit(0);
	}

	init_llvm(argv[1]); // setup LLVM stuff
	lexer_t *lexer = init_lexer(argv[1]);
	parser_t *parser = init_parser(lexer);

	init_table(); // initialize symbol_table
	parser_parse(parser);
	// print_table();
	dispose_llvm(); // dispose and free allocated memory for the LLVM compiler
	free(parser);
	free(lexer);

	return EXIT_SUCCESS;
}
