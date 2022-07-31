#include "arc.h"
#include "llvm_cfg.h"
#include "sym_table.h"
#include "parser.h"
#include "components/functions.h"

void build_fn(parser_t *parser)
{
	LLVMBuilderRef builder = llvm_cfg_get_builder();
	LLVMContextRef context = llvm_cfg_get_ctx();
	LLVMModuleRef module = llvm_cfg_get_module();
	LLVMTypeRef params[256] = {0};
	int arg_count = 0;

	consume(parser, TOKEN_IDENTIFIER, "expected a function name", "");
	char *fn_name = malloc(parser->previous->length * sizeof(char));
	sprintf(fn_name, "%.*s", parser->previous->length, parser->previous->start);
	consume(parser, TOKEN_LEFT_PAREN, "expected a '(' name", "try adding a `(` here");

	while (parser->current->type != TOKEN_RIGHT_PAREN)
	{
		consume(parser, TOKEN_IDENTIFIER, "expected argument name", "");
		char *name = malloc(parser->previous->length * sizeof(char));
		sprintf(name, "%.*s", parser->previous->length, parser->previous->start);

		consume(parser, TOKEN_COLON, "expected a colon", "try adding a `:` here");
		consume(parser, TOKEN_IDENTIFIER, "expected type name", "");

		char *param_type = malloc(parser->previous->length * sizeof(char));
		sprintf(param_type, "%.*s", parser->previous->length, parser->previous->start);
		LLVMTypeRef type;
		if (strcmp(param_type, "str") == 0)
		{
			type = LLVMInt32TypeInContext(context);
		}
		else if (strcmp(param_type, "int") == 0)
		{
			type = LLVMInt32TypeInContext(context);
		}
		else
		{
			type = LLVMInt32TypeInContext(context);
		}

		// printf("new arg :: %s\n", name);
		params[arg_count] = type;
		arg_count++;

		if (!match(parser, TOKEN_COMMA))
		{
			break;
		}
	}

	consume(parser, TOKEN_RIGHT_PAREN, "expected ')' 2", "try adding a `)` here");
	consume(parser, TOKEN_LEFT_BRACE, "expected '{'", "try adding a `{` here");

	LLVMTypeRef fn_function_type = LLVMFunctionType(LLVMInt8TypeInContext(context), params, arg_count, false);
	LLVMValueRef fn_type = LLVMAddFunction(module, fn_name, fn_function_type);

	LLVMBasicBlockRef fn_entry = LLVMAppendBasicBlockInContext(context, fn_type, "entry");
	LLVMPositionBuilderAtEnd(builder, fn_entry);

	while (!match(parser, TOKEN_RIGHT_BRACE))
	{
		declaration(parser);
	}

	set_symbol(SYMBOL_FN, fn_name, fn_type);

	LLVMValueRef main_function = LLVMGetNamedFunction(module, "main");
	LLVMBasicBlockRef main_entry = LLVMGetEntryBasicBlock(main_function);
	LLVMPositionBuilderAtEnd(builder, main_entry);
}
