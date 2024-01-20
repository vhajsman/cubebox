#ifndef _DEBUG_H
#define _DEBUG_H

#include "kernel/libs/krnlout.h"

#ifdef __DEBUG
	#define debug(msg)		{ print("DEBUG: "); print(msg);				 }
	#define debugln(msg)	{ print("DEBUG: "); print(msg); print("\n"); }
#else
	#define debug(msg)
	#define debugln(msg)
#endif

#endif