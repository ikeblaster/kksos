#include "freq.h"

	
std::string freq(std::string input) {
	const int MAX_VALUES = 256;
	int bytes[MAX_VALUES] = { 0 };
	char buff[65535] = {0};
	int curpos = 0;

	for (unsigned char c : input) {
		bytes[c]++;
	}

	for (int i = 0; i < MAX_VALUES; i++) {
		int act_num = bytes[i];

		if (act_num > 0) {
			curpos += _snprintf_s(buff + curpos, 65535 - curpos, _TRUNCATE, "0x%hhx : %d\n", i, act_num);
		}
	}

	return std::string(buff);
}
