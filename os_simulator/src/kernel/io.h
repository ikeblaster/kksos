#pragma once
#include "..\common\api.h"
#include "kernel.h"
#include "process.h"
#include "filesystem.filehandle.h"
#include "filesystem.pipe.h"

namespace IO
{
	void list_directory(std::vector<std::string> &items);
}

void HandleIO(CONTEXT &regs);