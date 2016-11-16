#include "vmstdio.h"

extern "C" size_t __stdcall dir(const CONTEXT &regs);


size_t __stdcall dir(const CONTEXT &regs)
{
	PROCESSSTARTUPINFO psi = *(PROCESSSTARTUPINFO*) regs.Rcx;

	if (psi.data.size() > 0) {
		std::string path = psi.data.at(0);
		Set_Cwd(path);
	}

	char *out_format = "%s\n";


	/* 
	//Curently unused
	for (int i = 0; i < psi.params.size(); i++) {
		char param = psi.params.at(i);
		
		switch (param) {
			case 'c':
				out_format = "%s\n";
				break;

		}

	}
	*/

	std::vector<std::string> items;
	List_Directory(items);

	for (auto item : items) {
		vmprintf(out_format, item.c_str());
	}

	return 0;
}
