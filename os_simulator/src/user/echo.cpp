#pragma once
#include <string>
#include <vector>
#include "..\common\api.h"
#include "vmstdio.h"

size_t __stdcall echo(const CONTEXT &regs)
{
	PROCESSSTARTUPINFO psi = *(PROCESSSTARTUPINFO*) regs.Rcx;

	for (int i = 0; i < psi.data.size(); i++) {
		vmprintf("%s ", psi.data[i].c_str());
	}

	vmprintf("\n");

	return 0;
}
