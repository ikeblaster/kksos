#include "vmstdio.h"


size_t __stdcall echo(const CONTEXT &regs)
{
	PROCESSSTARTUPINFO psi = *(PROCESSSTARTUPINFO*) regs.Rcx;

	for (int i = 0; i < psi.params.size(); i++) {
		vmprintf("%s ", psi.params[i].c_str());
	}

	vmprintf("\n");

	return 0;
}
