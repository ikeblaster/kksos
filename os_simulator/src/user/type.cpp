#include "vmstdio.h"


size_t __stdcall type(const CONTEXT &regs)
{
	PROCESSSTARTUPINFO psi = *(PROCESSSTARTUPINFO*) regs.Rcx;
	std::string filename = psi.data.at(0);
	auto testtxt = Create_File(filename.c_str(), FH_OPEN_EXISTING);

	if (testtxt == nullptr) return 0;

	while (true) {
		auto line = vmgetline(testtxt);
		if (line == nullptr) break;
		vmprintf("%s\n", line.get());
	}

	Close_File(testtxt);

	return 0;
}
