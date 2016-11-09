#pragma once
#include "..\common\api.h"
#include "filesystem.h"
#include "filesystem.consolehandle.h"
#include "io.h"
#include "process.h"

extern HMODULE User_Programs;

void Set_Error(const bool failed, CONTEXT &regs);

extern "C" void __stdcall SysCall(CONTEXT &regs);
extern "C" void __stdcall Run_VM();

void Initialize_FS();
