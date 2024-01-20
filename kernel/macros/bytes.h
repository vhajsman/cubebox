#ifndef _BYTES_H
#define _BYTES_H

#define BITS_IN_BYTE				8

#define BIT_SET(BYTE, N)			( (BYTE) |=  (1 << (N)) )
#define BIT_CLEAR(BYTE, N)			( (BYTE) &= ~(1 << (N)) )
#define BIT_TOGGLE(BYTE, N)			( (BYTE) ^=  (1 << (N)) )

#define BIT_IS_SET(BYTE, N)			( ((BYTE) &  (1 << (N))) != 0 )
#define BIT_IS_CLEAR(BYTE, N)		( ((BYTE) &  (1 << (N))) == 0 )

#define BIT_IS_EVEN(BYTE, N)		( ((BYTE) &   1) == 0 )
#define BIT_IS_ODD(BYTE, N)			( ((BYTE) &   1) != 0 )

#define BYTE_COUNT_SET_BITS(BYTE)	(	((BYTE) & 0x01) +			\
										((BYTE) >> 1 & 0x01)) +		\
										((BYTE) >> 2 & 0x01)) +		\
										((BYTE) >> 3 & 0x01)) +		\
										((BYTE) >> 4 & 0x01)) +		\
										((BYTE) >> 5 & 0x01)) +		\
										((BYTE) >> 6 & 0x01)) +		\
										((BYTE) >> 7 & 0x01)) )

#define BYTE_COUNT_CLEAR_BITS(BYTE)	(BITS_IN_BYTE - BYTE_COUNT_SET_BITS)

#define BYTE_REVERSE(BYTE)			( ((BYTE) * 0x0202020202ULL &	\
												0x010884422010ULL)	\
												% 1023 )

#define BYTE		unsigned char
#define WORD		unsigned short
#define DWORD		unsigned long
#define QWORD		unsigned long long

#define BYTE_MAX	0xFF
#define WORD_MAX	0xFFFF
#define DWORD_MAX	0xFFFFFF
#define QWORD_MAX	0xFFFFFFFF

#define BYTE_BITS	8
#define WORD_BITS	16
#define DWORD_BITS	32
#define QWORD_BITS	64

#endif
