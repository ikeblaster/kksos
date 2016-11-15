#pragma once
#include "ps.h"

size_t __stdcall ps(const CONTEXT &regs)
{
	PROCESSSTARTUPINFO psi = *(PROCESSSTARTUPINFO*)regs.Rcx;

	std::vector<std::string> items;
	List_Processes(items);

	for (auto item : items) {
		vmprintf("%s\n", item.c_str());
	}

	return 0;
}