#include <map>
#include "vmstdio.h"


size_t __stdcall sort(const CONTEXT &regs)
{
	PROCESSSTARTUPINFO psi = *(PROCESSSTARTUPINFO*) regs.Rcx;
	THandle input = THANDLE_STDIN;
	THandle textfile = nullptr;

	if (psi.data.size() > 0) {
		textfile = Create_File(psi.data.at(0).c_str(), FH_OPEN_EXISTING); // TODO: nevytvaret soubor, pokud neexistuje
		if (textfile != nullptr) input = textfile;
	}

	std::multimap<std::string, std::unique_ptr<const char[]>> lines;

	while(true) {
		auto line = vmgetline(input);
		if (line == nullptr) break;
		lines.insert(std::make_pair(line.get(), std::move(line))); // TODO: 1. parametr, razeni podle urciteho sloupce
	}

	if (textfile != nullptr) Close_File(textfile);

	for (const auto& kv : lines) {
		vmprintf("%s\n", kv.second.get());
	}

	return 0;
}
