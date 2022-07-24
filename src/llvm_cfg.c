#include "llvm_cfg.h"

LLVMExecutionEngineRef engine;
LLVMModuleRef module;
LLVMContextRef context;
LLVMBuilderRef builder;
LLVMPassManagerRef pass_manager;

LLVMExecutionEngineRef llvm_cfg_get_engine(){
	return engine;
}

LLVMModuleRef llvm_cfg_get_module(){
	return module;
}

LLVMContextRef llvm_cfg_get_ctx(){
	return context;
}

LLVMBuilderRef llvm_cfg_get_builder(){
	return builder;
}

LLVMPassManagerRef llvm_cfg_get_pass_manager(){
	return pass_manager;
}

void init_llvm(char* mod_name){
	module = LLVMModuleCreateWithName(mod_name);
	context = LLVMContextCreate();
	builder = LLVMCreateBuilder();

	char *msg;
	if (LLVMCreateExecutionEngineForModule(&engine, module, &msg) == 1)
	{
		fprintf(stderr, "%serror%s: %s\n", RED_BOLD, RESET, msg);
		LLVMDisposeMessage(msg);
	}


	LLVMInitializeNativeTarget();
	LLVMLinkInMCJIT();
	pass_manager = LLVMCreateFunctionPassManagerForModule(module);
	LLVMAddPromoteMemoryToRegisterPass(pass_manager);
	LLVMAddInstructionCombiningPass(pass_manager);
	LLVMAddReassociatePass(pass_manager);
	LLVMAddGVNPass(pass_manager);
	LLVMAddCFGSimplificationPass(pass_manager);
	LLVMInitializeFunctionPassManager(pass_manager);

	LLVMTypeRef main_function_type = LLVMFunctionType(LLVMInt32TypeInContext(context), NULL, 0, false);
	LLVMValueRef main_function = LLVMAddFunction(module, "main", main_function_type);

	// TODO: this is where standard libraries get included

	LLVMBasicBlockRef entry = LLVMAppendBasicBlockInContext(context, main_function, "entry");
	LLVMPositionBuilderAtEnd(builder, entry);
}

void dispose_llvm(){
	LLVMBuildRet(builder, LLVMConstInt(LLVMInt32TypeInContext(context), 0, false));
	LLVMDumpModule(module);
	// disposing the pass manager returns this weird error
	// where sometimes the code works
	// but sometimes it either gives a malloc error
	// or a segfault
	// LLVMDisposePassManager(pass_manager);
	LLVMDisposeBuilder(builder);
	LLVMDisposeModule(module);
}
