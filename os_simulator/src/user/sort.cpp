#include <map>
#include "vmstdio.h"


size_t __stdcall sort(const CONTEXT &regs)
{
	auto args = getArguments(regs);
	THandle input = THANDLE_STDIN;

	if (args->size() > 0) {
		input = Create_File(args->at(0), FH_OPEN_EXISTING | FH_SHARED_READ);
	}

	if (input == INVALID_THANDLE) {
		vmprintf(THANDLE_STDERR, "Unable to open file.\n");
		return 1;
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
