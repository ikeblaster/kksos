#pragma once
#include "..\common\api.h"
#include "kernel.h"
#include "process.h"
#include "filesystem.filehandle.h"
#include "filesystem.pipe.h"

void HandleIO(CONTEXT &regs);