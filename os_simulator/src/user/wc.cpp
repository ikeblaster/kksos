#include "vmstdio.h"


size_t __stdcall wc(const CONTEXT &regs)
{
	PROCESSSTARTUPINFO psi = *(PROCESSSTARTUPINFO*) regs.Rcx;
	THandle input = THANDLE_STDIN;

	if (psi.data.size() > 0) {
		input = Create_File(psi.data.at(0).c_str(), FH_OPEN_EXISTING); // TODO: nevytvaret soubor, pokud neexistuje
		if (input == nullptr) {
			vmprintf(THANDLE_STDERR, "Unable to open file.\n");
			return 0;
		}
	}

	int wordcount = 0;
	int linecount = 0;

	size_t read = 0;
	char buffer[1024];
	bool onspace = true;

	while (true) {
		if (!Read_File(input, (const void*) &buffer, 1024, read) || read == 0)
			break;

		if (linecount == 0) linecount++;

		for (size_t i = 0; i < read; i++) {
			unsigned char chr = buffer[i];

			if (chr == '\n') linecount++;

			if (isspace(chr) && !onspace) {
				onspace = true;
			}
			else if (!isspace(chr) && onspace) {
				wordcount++;
				onspace = false;
			}
		}
	}

	vmprintf("Word count: %d\nLine count: %d\n", wordcount, linecount);

	Close_File(input);

	return 0;
}
