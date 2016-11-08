#pragma once

#include "wc.h"

#include <thread>
#include <map>
#include <stdio.h>


size_t __stdcall wc(const CONTEXT &regs)
{
	PROCESSSTARTUPINFO psi = *(PROCESSSTARTUPINFO*) regs.Rcx;
	THandle input = psi.p_stdin;

	THandle textfile = Create_File(psi.data.at(0).c_str(), OPEN_EXISTING); // nahradte systemovym resenim, zatim viz Console u CreateFile na MSDN (Psal Vojtik tak to tak necham)
	if (textfile != nullptr) input = textfile;

	bool count_lines = true;

	int wordcount = 0;
	int linecount = 0;


	while (true) {

		auto line = vmgetline(input);
		if (line == nullptr) break;

		linecount++;

		const char* chr = line.get();

		// Skip over spaces at the beginning of the word
		while (isspace(*chr))
			chr++;


		for (; *chr != 0; chr++)
		{
			if (isspace(*chr))
			{
				wordcount++;

				chr++;

				// Skip over unnecessary spaces
				while (isspace(*chr)) 
					chr++;
			}
		}
		if(line.get()[0] != 0)
			wordcount++;
	}


	vmprintf("Word count: %d\nLine count: %d\n", wordcount, linecount);

	if (textfile != nullptr) Close_File(input);

	return 0;
}
