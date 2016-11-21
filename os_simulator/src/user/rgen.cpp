#include <random>
#include "vmstdio.h"


volatile bool generate;

extern "C" size_t __stdcall rgen__controlz(const CONTEXT &regs)
{
	char chr;
	size_t read;
	Probe_File(THANDLE_STDIN, PROBE__SET_CHARMODE);

	while (!Read_File(THANDLE_STDIN, (const void *) &chr, 1, read) && chr != CHAR_EOF);

	generate = false;

	Probe_File(THANDLE_STDIN, PROBE__SET_LINEMODE);
	return 0;
}


extern "C" size_t __stdcall rgen(const CONTEXT &regs)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0, 1);

	bool stdinIsRedirected = (Probe_File(THANDLE_STDIN, PROBE__IS_INTERACTIVE) == FALSE);

	if (stdinIsRedirected) 
	{
		vmprintf("%f\n", dis(gen));
	}
	else 
	{
		generate = true;
		pid_t process = Create_Subprocess((TEntryPoint) &rgen__controlz);

		while (generate)
			vmprintf("%f\n", dis(gen));

		Join_Process(process);
	}

	return 0;
}
