#ifndef _MACROS_DEBUG_H
#define _MACROS_DEBUG_H

#ifndef __DEBUG_ENABLE
	#define __DEBUG_ENABLE 0
#else
	#define __DEBUG_ENABLE 1
#endif

#if __DEBUG_ENABLE == 0
	#define debug(message)
#elif __DEBUG_ENABLE == 1
	#define debug(message) {}
#endif

#endif
