#include "sort.h"

#include <thread>
#include <map>
#include <stdio.h>


void call_from_thread() {
	vmprintf("Hello %s\n", "thread");
}


size_t __stdcall sort(const CONTEXT &regs)
{
	auto testtxt = Create_File("test.txt", OPEN_EXISTING); // nahradte systemovym resenim, zatim viz Console u CreateFile na MSDN
	size_t read;

	std::map<std::string, std::unique_ptr<const char[]>> lines;

	while(true) {
		auto line = vmgetline(testtxt, read);
		if (line == nullptr) break;
		lines.insert(std::make_pair(std::string(line.get()), std::move(line))); // TODO: razeni podle urciteho sloupce
	}

	Close_File(testtxt);

	for (const auto& kv : lines) {
		vmprintf("%s\n", kv.second.get());
	}

	return 0;
}
