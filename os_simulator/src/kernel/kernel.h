#pragma once
#include "..\common\api.h"
#include "filesystem.h"
#include "filesystem_utils.h"
#include "io.h"
#include "process.h"

extern HMODULE User_Programs;
extern FileSystem::Directory* fs_cwd; // TODO: bude se predavat v kontextu procesu, nebude zde; v kernelu musi byt jen nekde vytvoreni FS + C

void Set_Error(const bool failed, CONTEXT &regs);

extern "C" void __stdcall SysCall(CONTEXT &regs);
extern "C" void __stdcall Run_VM();

void Initialize_FS();
