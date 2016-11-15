#include <thread>
#include "vmstdio.h"


void call_from_thread(THandle handle) {
	char buffer[255] = { 0 };
	char *p_buffer = &buffer[0];
	size_t read;
	Read_File(handle, (const void **) &p_buffer, 255, read);
	buffer[read] = 0;

	vmprintf("thread %d: %s|\n", std::this_thread::get_id(), buffer);
}


float rgen() {
	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	return r;
}

float rgen(float from, float to) {
	float r = from + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (to - from));
	return r;
}

extern "C" size_t __stdcall rgen(const CONTEXT &regs) {
	vmprintf("%f", rgen());
	return 0;
}
