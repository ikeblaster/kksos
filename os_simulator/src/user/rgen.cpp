#include <thread>
#include "vmstdio.h"


bool generate;

extern "C" size_t __stdcall rgen__controlz(const CONTEXT &regs) {
	char chr;
	size_t read;
	Probe_File(THANDLE_STDIN, PROBE__SET_CHARMODE);

	while (!Read_File(THANDLE_STDIN, (const void *) &chr, 1, read) && chr != CHAR_EOF);

	generate = false;

	Probe_File(THANDLE_STDIN, PROBE__SET_LINEMODE);
	return 0;
}


float rgen() 
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

float rgen(float from, float to) 
{
	return from + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (to - from));
}

extern "C" size_t __stdcall rgen(const CONTEXT &regs)
{
	generate = true;
	pid_t process = Create_Subprocess((TEntryPoint) &rgen__controlz);

	while (generate)
		vmprintf("%f\n", rgen());

	Join_Process(process);

	return 0;
}
