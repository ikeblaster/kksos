#include "vmstdio.h"


std::string freq(std::string input) {
	int bytes[UCHAR_MAX] = { 0 };
	char buff[65535] = {0};
	int curpos = 0;

	// TODO: nacitat pres vmgetline() (nebo jeste lepe pres Read_File) a teprve to zpracovavat
	for (unsigned char c : input) {
		bytes[c]++; 
	}

	for (int i = 0; i < UCHAR_MAX; i++) {
		int act_num = bytes[i];

		if (act_num > 0) {
			curpos += _snprintf_s(buff + curpos, 65535 - curpos, _TRUNCATE, "0x%hhx : %d\n", i, act_num); // TODO: vmprintf
		}
	}

	return std::string(buff);
}


extern "C" size_t __stdcall freq(const CONTEXT &regs) {
	return 0; 
}
