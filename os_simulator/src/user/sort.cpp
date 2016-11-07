#include "sort.h"

#include <thread>
#include <map>
#include <stdio.h>


void call_from_thread() {
	vmprintf("Hello %s\n", "thread");
}


size_t __stdcall sort(const CONTEXT &regs)
{
	std::vector<std::string> args = *(std::vector<std::string>*) regs.Rcx; // TODO: cteni ze stdin, pokud chybi parametr

	auto testtxt = Create_File(args.at(0).c_str(), OPEN_EXISTING); // nahradte systemovym resenim, zatim viz Console u CreateFile na MSDN
	std::multimap<std::string, std::unique_ptr<const char[]>> lines; 

	while(true) {
		auto line = vmgetline(testtxt);
		if (line == nullptr) break;
		lines.insert(std::make_pair(line.get(), std::move(line))); // TODO: 1. parametr, razeni podle urciteho sloupce
	}

	Close_File(testtxt);

	for (const auto& kv : lines) {
		vmprintf("%s\n", kv.second.get());
	}

	return 0;
}
