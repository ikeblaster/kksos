#pragma once
#include "dir.h"

size_t __stdcall dir(const CONTEXT &regs)
{
	PROCESSSTARTUPINFO psi = *(PROCESSSTARTUPINFO*) regs.Rcx;

	if (psi.data.size() > 0) {
		std::string path = psi.data.at(0);
		//TODO: zmìnit cwd
		Set_Cwd(path);
	}

	//TODO: Zavolat vypsání složky. Vojtík na mì køièí, že už to dìlá.
	std::vector<std::string> items;
	List_Directory(items);

	vmprintf("Dir command execution.\n");

	return 0;
}
