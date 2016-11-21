#include "vmstdio.h"


extern "C" size_t __stdcall echo(const CONTEXT &regs)
{
	auto args = getArguments(regs);

	for (int i = 0; i < args->size(); i++) {
		vmprintf("%s ", args->at(i).c_str());
	}

	vmprintf("\n");

	return 0;
}
