#include "vmstdio.h"


extern "C" size_t __stdcall dir(const CONTEXT &regs)
{
	auto args = getArguments(regs);

	if (args->size() > 0) {
		std::string path = args->at(0);
		if (!Set_Cwd(path)) {
			vmprintf(THANDLE_STDERR, "Unable to open path.\n");
			return 1;
		}
	}

	std::vector<std::string> items;
	List_Directory(items);

	for (auto item : items) {
		vmprintf("%s\n", item.c_str());
	}

	return 0;
}
