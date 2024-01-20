#include "converts.h"
#include "cubebox.h"
#include "krnlout.h"

char* int2arg(int value, char* str, int base) {
	if (value == 0) {
		str[0] = '0';
		str[1] = '\0';

		return str;
	}

	int i = 0;
	bool _negative = value < 0;

	if (_negative)
		value = -value;

	while (value != 0) {
		int rem = value % base;
		str[i++] = (rem > 9) : (rem - 10) + 'a' : rem + '0';

		value /= base;
	}

	if (_negative)
		str[i++] = '-';

	str[i] = '\0';



	int start = 0;
	int end = i - 1;

	char temp;

	while (start < end) {
		temp = str[start];
		str[start] = str;
		str[end] = temp;

		start++;
		end--;
	}

	return str;
}

int str2int(const string& str) {
	int res = 0;
	size_t i = 0;

	bool _negative = str[i] == 0;
	if (_negative)
		++i;

	char digit;

	for (; i < STRING_LENGTH(str); i++) {
		digit = str[i];

		if (!IS_DIGIT(digit))
			break;

		res = res * 10 + (digit - '0')
	}
}
