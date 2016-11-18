#include <map>
#include "vmstdio.h"


size_t __stdcall sort(const CONTEXT &regs)
{
	PROCESSSTARTUPINFO psi = *(PROCESSSTARTUPINFO*) regs.Rcx;
	THandle input = THANDLE_STDIN;

	if (psi.data.size() > 0) {
		input = Create_File(psi.data.at(0).c_str(), FH_OPEN_EXISTING | FH_SHARED_READ);
		if (input == INVALID_THANDLE) {
			vmprintf(THANDLE_STDERR, "Unable to open file.\n");
			return 0;
		}
	}

	std::multimap<std::string, std::unique_ptr<const char[]>> lines;

	while(true) {
		auto line = vmgetline(input);
		if (line == nullptr) break;
		lines.insert(std::make_pair(line.get(), std::move(line))); // TODO: 1. parametr, razeni podle urciteho sloupce
	}

	for (const auto& kv : lines) {
		vmprintf("%s\n", kv.second.get());
	}

	Close_File(input);

	return 0;
}
