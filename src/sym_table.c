#include "sym_table.h"

symbol_t *symbols[TABLE_SIZE];

int hash(char *name)
{
	int length = strlen(name);
	int hash_value = 0;
	for (int i = 0; i < length; i++)
	{
		hash_value += name[i];
		hash_value = (hash_value * name[i]) % TABLE_SIZE;
	}

	return hash_value;
}

void init_table()
{
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		symbols[i] = NULL;
	}
}

void print_table()
{
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		if (symbols[i] == NULL)
		{
			printf("\t%d\t---\t---\n", i);
		}
		else
		{
			printf("\t%d\t%d\t%s\n", i, symbols[i]->type, symbols[i]->name);
		}
	}
}

symbol_t *get_symbol(char *name)
{
	int idx = hash(name);
	if (symbols[idx] != NULL && strcmp(symbols[idx]->name, name) == 0)
	{
		return symbols[idx];
	}
	else
	{
		return NULL;
	}
}

void set_symbol(symbol_type type, char *name, LLVMValueRef value)
{
	int key = hash(name);
	while (symbols[key] != NULL)
	{
		// TODO: return a err status code if unable to set
		// 		 a symbol
		return;
	}
	symbol_t *sym = malloc(sizeof(symbol_t));
	sym->type = type;
	sym->name = malloc(sizeof(char) * strlen(name));
	strcpy(sym->name, name);
	sym->value = value;
	symbols[key] = sym;
}
