#ifndef SYM_TABLE_H
#define SYM_TABLE_H

#include "arc.h"
#include "llvm_cfg.h"

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
void set_symbol(symbol_type type, char* name, LLVMValueRef value);

#endif // SYM_TABLE_H
