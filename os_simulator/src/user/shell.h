#pragma once
#include "..\common\api.h"
#include "rtl.h"
#include "parser.h"
#include "vmstdio.h"

extern "C" size_t __stdcall shell(const CONTEXT &regs);