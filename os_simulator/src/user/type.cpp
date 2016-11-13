#pragma once

#include "type.h"



size_t __stdcall type(const CONTEXT &regs)
{
	PROCESSSTARTUPINFO psi = *(PROCESSSTARTUPINFO*) regs.Rcx;
	std::string filename = psi.data.at(0);
	auto testtxt = Create_File(filename.c_str(), FH_OPEN_EXISTING); // nahradte systemovym resenim, zatim viz Console u CreateFile na MSDN (Psal Vojtik tak to tak necham)

	while (true) {
		auto line = vmgetline(testtxt);
		if (line == nullptr) break;
		vmprintf("%s\n", line.get());
	}

	Close_File(testtxt);

	return 0;
}
