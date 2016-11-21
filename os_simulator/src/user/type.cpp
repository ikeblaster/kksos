#include "vmstdio.h"


extern "C" size_t __stdcall type(const CONTEXT &regs)
{
	auto args = getArguments(regs);
	THandle input = INVALID_THANDLE;

	if (args->size() > 0) {
		input = Create_File(args->at(0), FH_OPEN_EXISTING | FH_SHARED_READ);
	}

	if (input == INVALID_THANDLE) {
		vmprintf(THANDLE_STDERR, "Unable to open file.\n");
		return 1;
	}

	size_t read = 0;
	unsigned char buffer[1024];

	while (true) {
		if (!Read_File(input, (const void*) &buffer, 1024, read) || read == 0)
			break;

		Write_File(THANDLE_STDOUT, buffer, read, read);
	}

	Close_File(input);

	return 0;
}
