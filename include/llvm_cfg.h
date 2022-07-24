#ifndef LLVM_CFG_H
#define LLVM_CFG_H

#include "arc.h"
#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <llvm-c/Transforms/Scalar.h>
#include <llvm-c/Transforms/Utils.h>


void init_llvm(char* mod_name);
void dispose_llvm();

LLVMExecutionEngineRef llvm_cfg_get_engine();
LLVMModuleRef llvm_cfg_get_module();
LLVMContextRef llvm_cfg_get_ctx();
LLVMBuilderRef llvm_cfg_get_builder();
LLVMPassManagerRef llvm_cfg_get_pass_manager();

#endif // LLVM_CFG_H
