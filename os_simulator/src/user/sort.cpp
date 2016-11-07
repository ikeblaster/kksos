#include "sort.h"

#include <thread>
#include <map>
#include <stdio.h>

THandle pipe;

void call_from_thread(THandle handle) {
	char buffer[255] = { 0 };
	char *p_buffer = &buffer[0];
	size_t read;
	Read_File(handle, (const void **) &p_buffer, 255, read);
	buffer[read] = 0;

	vmprintf("thread %d: %s|\n", std::this_thread::get_id(), buffer);
}


size_t __stdcall sort(const CONTEXT &regs)
{
	THandle pipe = Create_File(nullptr, IHANDLE_PIPE); // TODO: ziskat stdout aktualniho procesu
	std::thread thr(call_from_thread, pipe);

	vmprintf(pipe, "%s%c", "ahoj", 26);

	thr.join();

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
