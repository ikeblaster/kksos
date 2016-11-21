#include "vmstdio.h"


extern "C" size_t __stdcall ps(const CONTEXT &regs)
{
	std::vector<std::string> items;
	List_Processes(items);

	for (auto item : items) {
		vmprintf("%s\n", item.c_str());
	}

	return 0;
}
