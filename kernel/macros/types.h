#ifndef _TYPES_H
#define _TYPES_H

#include "arrays.h"

typedef long unsigned int	size_t;
typedef long int			ptrdiff_t;
typedef int					wchar_t;
typedef int					wint_t;

// --- UINTX_T, INT_T types ---

#include "bytes.h"

typedef unsigned char		uint8_t;
typedef unsigned short		uint16_t;
typedef unsigned int		uint32_t;
typedef unsigned long long	uint64_t;

typedef signed char			int8_t;
typedef signed short		int16_t;
typedef signed int			int32_t;
typedef unsigned long long	int64_t;

typedef uint64_t			uint128_t[2];
typedef uint64_t			uint256_t[4];
typedef uint64_t			uint512_t[8];
typedef uint64_t			uint1024_t[16];

#define U8U16(X, Y)			(((uint16_t)   Y << 8 )  | X)
#define U16U32(X, Y)		(((uint32_t)   Y << 16)  | X)
#define U32U64(X, Y)		(((uint64_t)   Y << 32)  | X)
#define I8I16(X, Y)			(((int16_t)    Y << 8 )  | X)
#define I16I32(X, Y)		(((int32_t)    Y << 16)  | X)
#define I32I64(X, Y)		(((int64_t)    Y << 32)  | X)

#define U64U128(X, Y)		(((uint128_t)  Y << 64)  | X)
#define U128U256(X, Y)		(((uint256_t)  Y << 128) | X)
#define U256U512(X, Y)		(((uint512_t)  Y << 256) | X)
#define U512U1024(X, Y)		(((uint1024_t) Y << 512) | X)

#define U8_MAX				BYTE_MAX
#define U16_MAX				WORD_MAX
#define U32_MAX				DWORD_MAX
#define U64_MAX				QWORD_MAX

#define U8_MIN				0
#define U16_MIN				0
#define U32_MIN				0
#define U64_MIN				0

#define U8					uint8_t
#define U16					uint16_t
#define U32					uint32_t
#define U64					uint64_t

#define I8					int8_t
#define I16					int16_t
#define I32					int32_t
#define I64					int64_t

#define U8MERGE(X, Y)		(X | Y << 4)
#define U16MERGE(X, Y)		(X | Y << 8)
#define U32MERGE(X, Y)		(X | Y << 16)
#define U64MERGE(X, Y)		(X | Y << 32)

// --- booleans ---
typedef unsigned int		bool;

#define true				((bool) 1)
#define false				((bool) 0)

// --- strings --

typedef const char*			string;
#define STRING_LENGTH(STR)	(ARRAY_LEN(STR));
#define STRING_REVERSE(STR, R)	\
		ARRAY_REVERSE(STR, R)
#define STRING_CONCAT(S1, S2, R) \
		ARRAY_JOIN(S1, S2, R)
#endif
