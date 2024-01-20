#ifndef _KERNEL_H
#define _KERNEL_H

#define KERNEL_VERSION_STRING	"0.01.0-alpha"
#define KERNEL_VERSION_INT		((int) 0x0001ff)

#define VERIFY_COMPILER							\
	#if defined(__linux__)						\
		#error "Cross-compiler required."		\
	#endif										\
												\
	#if !defined(__i386__)						\
		#error "ix86-elf compiler required."	\
	#endif

#include "kernel/macros/general_constants.h"
#include "kernel/macros/types.h"
#include "kernel/macros/bytes.h"
#include "kernel/macros/debug.h"

#include "kernel/common/io.h"

#define OFFSET_OF(ST, M)			((size_t) &(((ST *) 0)->M))
#define offsetof(STRUCT, MEMBER)	OFFSET_OF(STRUCT, M)

#endif
