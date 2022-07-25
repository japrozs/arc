#include "parser.h"
#include "arc.h"
#include "components/variables.h"
#include "sym_table.h"

void build_str_var(parser_t *parser, char *name)
{
	// get the necessary LLVM components
	LLVMModuleRef module = llvm_cfg_get_module();

	if (match(parser, TOKEN_SEMICOLON))
	{
		LLVMTypeRef str_type = LLVMArrayType(LLVMInt8Type(), 0);
		LLVMValueRef str = LLVMAddGlobal(module, str_type, name);
		LLVMSetInitializer(str, LLVMConstString("", 0, true));
		LLVMSetGlobalConstant(str, false);

		set_symbol(SYMBOL_VAR, name, str);
		return;
	}
	consume(parser, TOKEN_EQUAL, "expected equal sign for assignment");
	consume(parser, TOKEN_STRING, "expected a string");

	char *value = malloc(parser->previous->length * sizeof(char));
	sprintf(value, "%s", parser->previous->start);

	LLVMTypeRef str_type = LLVMArrayType(LLVMInt8Type(), strlen(value));
	LLVMValueRef str = LLVMAddGlobal(module, str_type, name);
	LLVMSetInitializer(str, LLVMConstString(value, strlen(value), true));
	LLVMSetGlobalConstant(str, false);

	// DONE: add str to the symtable
	set_symbol(SYMBOL_VAR, name, str);

	consume(parser, TOKEN_SEMICOLON, "expected a semicolon");
}

void build_int_var(parser_t *parser, char *name)
{
	LLVMBuilderRef builder = llvm_cfg_get_builder();
	if (match(parser, TOKEN_SEMICOLON))
	{
		LLVMValueRef var = LLVMBuildAlloca(builder, LLVMInt32Type(), name);
		// DONE: add var to symbol table
		set_symbol(SYMBOL_VAR, name, var);
		return;
	}

	consume(parser, TOKEN_EQUAL, "expected equal sign for assignment");
	consume(parser, TOKEN_NUMBER, "expected value for variable with type int");
	char *value = malloc(parser->previous->length * sizeof(char));
	sprintf(value, "%s", parser->previous->start);

	LLVMValueRef var = LLVMBuildAlloca(builder, LLVMInt32Type(), name);
	LLVMBuildStore(builder, LLVMConstInt(LLVMInt32Type(), atoi(value), false), var);
	set_symbol(SYMBOL_VAR, name, var);

	consume(parser, TOKEN_SEMICOLON, "expected semicolon");
}

// void build_float_var(parser_t *parser, char *name)
// {
// 	LLVMBuilderRef builder = llvm_cfg_get_builder();
// 	LLVMModuleRef module = llvm_cfg_get_module();

// 	if (match(parser, TOKEN_SEMICOLON))
// 	{
// 		LLVMValueRef var = LLVMBuildAlloca(builder, LLVMFloatType(), name);
// 		// todo: add var to symbol table
// 		// LLVMBuildStore(builder, LLVMConstInt(LLVMInt32Type(), atoi(value), false), var);
// 		return;
// 	}
// 	consume(parser, TOKEN_EQUAL, "expected equal sign");
// 	consume(parser, TOKEN_FLOAT, "expected number");

// 	char *value = malloc(parser->previous->length * sizeof(char));
// 	sprintf(value, "%s", parser->previous->start);

// 	// todo: add var to symbol table
// 	LLVMTypeRef str_type = LLVMArrayType(LLVMInt8Type(), strlen(value));
// 	LLVMValueRef str = LLVMAddGlobal(module, str_type, name);
// 	LLVMSetInitializer(str, LLVMConstString(value, strlen(value), true));
// 	LLVMSetGlobalConstant(str, false);

// 	LLVMValueRef float_num = LLVMAddGlobal(module, LLVMFloatType(), name);
// 	LLVMSetInitializer(float_num, LLVMFP, true);

// 	consume(parser, TOKEN_SEMICOLON, "expected semicolon");
// }

// TODO: add variables to a symbol table
// gotta implement a symtable first
void build_var(parser_t *parser)
{
	consume(parser, TOKEN_IDENTIFIER, "expected a variable name");
	// store variable name
	char *var_name = malloc(parser->previous->length * sizeof(char));
	sprintf(var_name, "%.*s", parser->previous->length, parser->previous->start);

	consume(parser, TOKEN_COLON, "expected a colon");
	consume(parser, TOKEN_IDENTIFIER, "expected type name");

	char *var_type = malloc(parser->previous->length * sizeof(char));
	sprintf(var_type, "%.*s", parser->previous->length, parser->previous->start);

	if (strcmp(var_type, STR_TYPE) == 0)
	{
		build_str_var(parser, var_name);
	}
	else if (strcmp(var_type, INT_TYPE) == 0)
	{
		build_int_var(parser, var_name);
	}
	// else if (strcmp(var_type, FLOAT_TYPE) == 0)
	// {
	// 	build_float_var(parser, var_name);
	// }
}
