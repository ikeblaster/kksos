#include "vmstdio.h"

size_t __stdcall freq(const CONTEXT &regs) {	
	PROCESSSTARTUPINFO psi = *(PROCESSSTARTUPINFO*) regs.Rcx;
	THandle input = THANDLE_STDIN;
	int bytes[UCHAR_MAX] = { 0 };
	unsigned char buff[1024];	
	size_t read = 0;

	if (psi.data.size() > 0) {
		input = Create_File(psi.data.at(0).c_str(), FH_OPEN_EXISTING); // TODO: nevytvaret soubor, pokud neexistuje
		if (input == nullptr) {
			vmprintf(THANDLE_STDERR, "Unable to open file.\n");
			return 0;
		}
	}

	/* Stdin loading and process individual characters count */
	while (true) {
		if (!Read_File(input, (const void*) &buff, 1024, read) || read == 0)
			break;
		
		for (size_t i = 0; i < read; i++) {
			bytes[buff[i]]++;
		}
	}

	/* prints freq table into stdout without null (0x0) */
	for (int i = 1; i < UCHAR_MAX; i++) {
		int act_num = bytes[i];

		if (act_num > 0) {
			vmprintf("0x%hhx : %d\n", i, act_num);			
		}
	}

	Close_File(input);

	return 0; 
}
