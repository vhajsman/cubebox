#ifndef _MODULE_H
#define _MODULE_H

#include "cubebox.h"
#include "kernel/libs/krnlout.h"
#include "kernel/libs/krnlstatus.h"

struct _moduleCall {
	string		parentModule;
	uint32_t	callParams;
	uint16_t*	returnCode_Ptr;
};

#define MODULE(NAME)							\
	string __module_name = #NAME

#define MODULE_INIT(DEP, PARAMS, PTR)			\
	DEP##_init(struct _moduleCall {				\
		parentModule = __module_name;			\
		callParams = PARAMS;					\
		returnCode_Ptr = PTR;					\
	})
#define MODULE_DEINIT(DEP, PARAMS, PTR)			\
	DEP##_deinit(struct _moduleCall {			\
		parentModule = __module_name;			\
		callParams = PARAMS;					\
		returnCode_Ptr = PTR;					\
	})

#define MODULE_INITIALIZER(NAME)				\
	void NAME##_init (struct _moduleCall call)
#define MODULE_DEINITIALIZER(NAME)				\
	void NAME##_deinit (struct _moduleCall call)

#define MODULE_INIT_ITSELF(PARAMS)				\
	MODULE_INIT(__module_name, PARAMS);
#define MODULE_DEINIT_ITSELF(PARAMS)			\
	MODULE_DEINIT(__module_name, PARAMS);

#define KERNEL_MAIN_AS_MODULE					\
	MODULE("kmain");

#define MODULE_PRINT(MSG) {						\
	print("[");									\
	print(__module_name);						\
	print("] ");								\
	print(MSG);									\
}
#define MODULE_PRINTLN(MSG) {					\
	MODULE_PRINT(MSG);							\
	println();									\
}

#define MODULE_STATUS(STATUS, MSG, CODE)		\
	printStatusMsg(STATUS, __module_name, MSG, CODE);
#define MODULE_INIT_OK {									\
	MODULE_STATUS(KRNL_STATUS_OKAY, "Module init.", 200);	\
	return;													\
}
#ifdef MODULE_CRITICAL
#define MODULE_INIT_FAIL {									\
		MODULE_STATUS(KRNL_STATUS_FATAL, "Module init.", 600);	\
		return;													\
	}
#else 
#define MODULE_INIT_FAIL {									\
		MODULE_STATUS(KRNL_STATUS_FAIL, "Module init.", 500);	\
		return;													\
	}
#endif

#define MODULE_CALL(_MODULE, FUNCT, PARAMS)		\
	_MODULE##_##FUNCT(struct _moduleCall {		\
		parentModule = __module_name;			\
		callParams = PARAMS;					\
		returnCode_Ptr = nullptr;				\
	});

#define MODULE_FUNCT(_MODULE, FUNCT)			\
	static uint16_t _MODULE##_##FUNCT(struct _moduleCall call)

#define MODULE_RET(X) {							\
	&call.returnCode_Ptr = X;					\
	return;										\
}

#endif