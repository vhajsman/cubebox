#ifndef _LIBK_CONVERTS
#define _LIBK_CONVERTS

#define BINARY 2
#define DECIMAL 10
#define OCTADECIMAL 12
#define HEXADECIMAL 16

#define IS_DIGIT(X)		(X > ('0' - 1) && X < ('9' + 1))

char* int2arg(int value, char* str, int base);
int   str2int(const string& str)

#endif
