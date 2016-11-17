#include "vmstdio.h"


size_t __stdcall wc(const CONTEXT &regs)
{
	PROCESSSTARTUPINFO psi = *(PROCESSSTARTUPINFO*) regs.Rcx;
	THandle input = THANDLE_STDIN;

	if (psi.data.size() > 0) {
		input = Create_File(psi.data.at(0).c_str(), FH_OPEN_EXISTING);
		if (input == nullptr) {
			vmprintf(THANDLE_STDERR, "Unable to open file.\n");
			return 0;
		}
	}

	size_t charcount = 0;
	size_t wordcount = 0;
	size_t linecount = 0;

	size_t read = 0;
	unsigned char buffer[1024];
	bool onspace = true;

	while (true) {
		if (!Read_File(input, (const void*) &buffer, 1024, read) || read == 0)
			break;

		charcount += read;
		if (linecount == 0) linecount++;

		for (size_t i = 0; i < read; i++) {
			if (buffer[i] == '\n') linecount++;

			if (isspace(buffer[i]) && !onspace) {
				onspace = true;
			}
			else if (!isspace(buffer[i]) && onspace) {
				wordcount++;
				onspace = false;
			}
		}
	}

	vmprintf("Word count: %ld\nLine count: %ld\nCharacter count: %ld\n", wordcount, linecount, charcount);

	Close_File(input);

	return 0;
}
