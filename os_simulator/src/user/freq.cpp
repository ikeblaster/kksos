#include "vmstdio.h"


extern "C" size_t __stdcall freq(const CONTEXT &regs) 
{
	auto args = getArguments(regs);
	THandle input = THANDLE_STDIN;

	if (args->size() > 0) {
		input = Create_File(args->at(0), FH_OPEN_EXISTING);
	}

	if (input == INVALID_THANDLE) {
		vmprintf(THANDLE_STDERR, "Unable to open file.\n");
		return 1;
	}

	int bytes[UCHAR_MAX] = { 0 };
	unsigned char buff[1024];
	size_t read = 0;

	// load and process individual characters
	while (Read_File(input, (const void*) &buff, 1024, read) && read > 0) {
		for (size_t i = 0; i < read; i++) {
			bytes[buff[i]]++;
		}
	}

	// prints freq table
	for (int i = 1; i < UCHAR_MAX; i++) {
		if (bytes[i] > 0) {
			vmprintf("0x%hhx : %d\n", i, bytes[i]);
		}
	}

	Close_File(input);

	return 0; 
}
