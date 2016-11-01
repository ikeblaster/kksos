#include "rgen.h"
#include <stdlib.h>
#include <time.h>

float rgen() {
	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	return r;
}

float rgen(float from, float to) {
	float r = from + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (to - from));
	return r;
}
