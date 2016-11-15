#include "vmstdio.h"


size_t __stdcall wc(const CONTEXT &regs)
{
	PROCESSSTARTUPINFO psi = *(PROCESSSTARTUPINFO*) regs.Rcx;
	THandle input = THANDLE_STDIN;
	THandle textfile = nullptr;

	if (psi.data.size() > 0) {
		textfile = Create_File(psi.data.at(0).c_str(), FH_OPEN_EXISTING); // TODO: nevytvaret soubor, pokud neexistuje
		if (textfile != nullptr) input = textfile;
	}

	bool count_lines = true;

	int wordcount = 0;
	int linecount = 0;


	while(true) {
		auto line = vmgetline(input);
		if (line == nullptr) break;

		linecount++;

		const char* chr = line.get();

		// Skip over spaces at the beginning of the word
		while(isspace(*chr) && *chr != 0)
			chr++;

		while(*chr != 0)
		{
			if (isspace(*chr))
			{
				wordcount++;

				// Skip over unnecessary spaces
				while (isspace(*chr) && *chr != 0)
					chr++;
			}
			else
				chr++;
		}
		if(line.get()[0] != 0)
			wordcount++;
	}


	vmprintf("Word count: %d\nLine count: %d\n", wordcount, linecount);

	if (textfile != nullptr) Close_File(input);

	return 0;
}
