#ifndef _LIBS_STRINGS
#define _LIBS_STRINGS

#include "cubebox.h"

#define UPPERCASE_START	65
#define UPPERCASE_END	90

#define LOWERCASE_START	97
#define LOWERCASE_END	122

#define LETTERS			(UPPERCASE_END - UPPERCASE_START)

#define IS_UPPERCASE(CHAR)	(CHAR > (UPPERCASE_START - 1) && (CHAR < (UPPERCASE_END)))
#define IS_LOWERCASE(CHAR)	(CHAR > (LOWERCASE_START - 1) && (CHAR < (LOWERCASE_END)))
#define IS_LETTER(CHAR)		(IS_UPPERCASE(CHAR) || IS_LOWERCASE(CHAR))

extern const char* _chars_uppercase;
extern const char* _chars_lowercase;

string toUpperCase(string str);
string toLowerCase(string str);

void splitStringByChar(string str, char c, string& resultArray_Ptr[]);

void* memmove(void* dstptr, const void* srcptr, size_t size);
int memcmp(const void* aptr, const void* bptr, size_t size);
void* memset(void* bufptr, int value, size_t size);
void* memcpy(void* restrict dstptr, const void* restrict srcptr, size_t size);

#endif
