#pragma once
#include "dir.h"

size_t __stdcall dir(const CONTEXT &regs)
{
	PROCESSSTARTUPINFO psi = *(PROCESSSTARTUPINFO*) regs.Rcx;

	if (psi.data.size() > 0) {
		std::string path = psi.data.at(0);
		Set_Cwd(path);
	}

	std::vector<std::string> items;
	List_Directory(items);

	for (auto item : items) {
		vmprintf("%s\n", item.c_str());
	}

	return 0;
}
