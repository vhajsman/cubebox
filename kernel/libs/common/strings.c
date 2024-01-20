#include "strings.h"

const char* _chars_uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char* _chars_lowercase = "abcdefghijklmnopqrstuvwxyz";

string toUpperCase(string str) {
	string result;

	for (int i = 0; i < STRING_LENGTH(str); i++) {
		if (IS_LOWERCASE(str[i])) {
			result[i] = str[i] - LETTERS - 6;
		}
		else {
			result[i] = str[i];
		}
	}

	return result;
}

string toLowerCase(string str) {
	string result;

	for (int i = 0; i < STRING_LENGTH(str); i++) {
		if (IS_UPPERCASE(str[i])) {
			result[i] = str[i] + LETTERS + 6;
		}
		else {
			result[i] = str[i];
		}
	}

	return result;
}

void splitStringByChar(string str, char c, string& resultArray_Ptr[]) {
	int resinx = 0;
	int i2 = 0;
	
	for (size_t i = 0; i < STRING_LENGTH(str); i++) {
		if (str[i] == c) {
			i2 = 0;

			if (resinx) 
				resinx++;

			continue;
		}

		resultArray_Ptr[resinx][i2] = str[i];
		
		i2++;
	}
}

void* memmove(void* dstptr, const void* srcptr, size_t size) {
	unsigned char* dst =		(unsigned char*)		dstptr;
	const unsigned char* src =	(const unsigned char*)	srcptr;

	if (dst < src) {
		for (size_t i = 0; i < size; i++)
			dst[i] = src[i];
	} else {
		for (size_t i = size; i != 0; i--)
			dst[i - 1] = src[i - 1];
	}

	return dstptr;
}

int memcmp(const void* aptr, const void* bptr, size_t size) {
	const unsigned char* a = (const unsigned char*) aptr;
	const unsigned char* b = (const unsigned char*) bptr;

	for (size_t i = 0; i < size; i++) {
		if (a[i] < b[i]) {
			return -1;
		} else if (b[i] < a[i]) {
			return 1;
		}
	}

	return 0;
}

void* memset(void* bufptr, int value, size_t size) {
	unsigned char* buf = (unsigned char*) bufptr;

	for (size_t i = 0; i < size; i++)
		buf[i] = (unsigned char) value;

	return bufptr;
}

void* memcpy(void* restrict dstptr, const void* restrict srcptr, size_t size) {
	unsigned char* dst =		(unsigned char*)		dstptr;
	const unsigned char* src =	(const unsigned char*)	srcptr;

	for (size_t i = 0; i < size; i++)
		dst[i] = src[i];

	return dstptr;
}
