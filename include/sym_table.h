#ifndef SYM_TABLE_H
#define SYM_TABLE_H

#include "arc.h"
#include "llvm_cfg.h"

#define TABLE_SIZE 20

typedef enum
{
	SYMBOL_VAR,
	SYMBOL_FN
} symbol_type;

typedef struct
{
	char *name;
	symbol_type type;
	LLVMValueRef value;
} symbol_t;

void init_table();
void set_symbol(symbol_type type, char *name, LLVMValueRef value);
symbol_t *get_symbol(char *name);
void print_table();

#endif // SYM_TABLE_H
