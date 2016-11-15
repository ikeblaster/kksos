#pragma once
#include "echo.h"

size_t __stdcall echo(const CONTEXT &regs)
{
	PROCESSSTARTUPINFO psi = *(PROCESSSTARTUPINFO*) regs.Rcx;

	for (int i = 0; i < psi.data.size(); i++) {
		vmprintf("%s ", psi.data[i].c_str());
	}

	vmprintf("\n");

	return 0;
}
