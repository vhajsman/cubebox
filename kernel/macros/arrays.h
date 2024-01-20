#ifndef _ARRAYS_H
#define _ARRAYS_H

#include "cubebox.h"

#define MAKE_STATIC_ARRAY(TYPE, SIZE, NAME)		static TYPE NAME[SIZE]
#define ARRAY_LEN(ARRAY)						(sizeof(ARRAY) / sizeof(ARRAY[0]))

#define ARRAY_FILL(ARRAY, VALUE) {																			\
	for(int __iterator = 0; __iterator < ARRAY_LEN(ARRAY) && __iterator < MAX_ITERATOR; __iterator ++)		\
		ARRAY[__iterator] = VALUE;																			\
}

// Finds index of highest value in array
#define ARRAY_MAX_INDEX(ARRAY, _PTR) {																		\
	int lidx, lval = 0;																						\
	for(int __iterator = 0; __iterator < ARRAY_LEN(ARRAY) && __iterator < MAX_ITERATOR; __iterator ++){		\
		if(ARRAY[__iterator] > lval) {																		\
			lval = ARRAY[__iterator];																		\
			lidx = __iterator;																				\
		}																									\
	}																										\
																											\
	_PTR = lidx;																							\
}

// Finds index of lowest value in array
#define ARRAY_MIN_INDEX(ARRAY, _PTR) {																		\
	int lidx, lval = 1000;																					\
	for(int __iterator = 0; __iterator < ARRAY_LEN(ARRAY) && __iterator < MAX_ITERATOR; __iterator ++){		\
		if(ARRAY[__iterator] < lval) {																		\
			lval = ARRAY[__iterator];																		\
			lidx = __iterator;																				\
		}																									\
	}																										\
																											\
	_PTR = lidx;																							\
}

#define ARRAY_SUM(ARRAY, _PTR) {																			\
	int sum = 0;																							\
	for(int __iterator = 0; __iterator < ARRAY_LEN(ARRAY) && __iterator < MAX_ITERATOR; __iterator ++)		\
		sum = sum + ARRAY[__iterator];																		\
																											\
	_PTR = sum;																								\
	delete sum;																								\
}

#define ARRAY_AVERAGE(ARRAY, _PTR) {																		\
	double avg = 0;																							\
																											\
	ARRAY_SUM(ARRAY, avg);																					\
	avg = avg / ARRAY_LEN(ARRAY);																			\
																											\
	_PTR = avg;																								\
	delete avg;																								\
}

#define ARRAY_REVERSE(ARRAY, _PTR) {																		\
	for(int __iterator = 0; __iterator < ARRAY_LEN(ARRAY) && __iterator < MAX_ITERATOR; __iterator ++)		\
		_PTR[ARRAY_LEN(ARRAY) - __iterator] = ARRAY[__iterator];											\
}

#define ARRAY_CREATE_INITIALIZED(TYPE, SIZE, NAME, VALUE) {													\
	MAKE_STATIC_ARRAY(TYPE, SIZE, NAME);																	\
	ARRAY_FILL(ARRAY, VALUE);																				\
}

#define ARRAY_JOIN(ARRAY1, ARRAY2, TYPE, RESULT) {															\
	if(sizeof(ARRAY1[0]) == sizeof(ARRAY2[0])) {															\
		MAKE_STATIC_ARRAY(TYPE, ARRAY_LEN(ARRAY1) + ARRAY_LEN(ARRAY2), RESULT);								\
																											\
		for(int __iterator = 0; __iterator < ARRAY_LEN(ARRAY1) && __iterator < MAX_ITERATOR; __iterator ++)	\
			RESULT[__iterator] = ARRAY1[__iterator];														\
																											\
		for (int __iterator = 0; __iterator < ARRAY_LEN(ARRAY1) && __iterator < MAX_ITERATOR; __iterator++)	\
			RESULT[__iterator + ARRAY_LEN(ARRAY1)] = ARRAY2[__iterator];									\
	} else {																								\
		#error "Type error."																				\
	}																										\
}
	

#endif