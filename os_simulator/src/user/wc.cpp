#pragma once

#include "wc.h"

#include <thread>
#include <map>
#include <stdio.h>


size_t __stdcall wc(const CONTEXT &regs)
{
	std::vector<std::string> args = *(std::vector<std::string>*) regs.Rcx;
	auto testtxt = Create_File(args.at(0).c_str(), OPEN_EXISTING); // nahradte systemovym resenim, zatim viz Console u CreateFile na MSDN (Psal Vojtik tak to tak necham)

	bool count_lines = true;

	int wordcount = 0;
	int linecount = 0;


	while (true) {

		auto line = vmgetline(testtxt);
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

	Close_File(testtxt);

	return 0;
}
