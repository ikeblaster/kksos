#pragma once

#include "kernel.h"
#include "io.h"
#include "filesystem.h"

HMODULE User_Programs;

void Set_Error(const bool failed, CONTEXT &regs) {
	if (failed) {
		stc(regs.EFlags);
		regs.Rax = GetLastError();
	}
	else
		clc(regs.EFlags);
}


void Initialize_Kernel() {
	User_Programs = LoadLibrary(L"user.dll");	
}

void Shutdown_Kernel() {
	FreeLibrary(User_Programs);
}

void SysCall(CONTEXT &regs) {

	switch (Get_AH((__int16) regs.Rax)) {
		case scIO:		HandleIO(regs);
	}

}

using namespace FS;

void Run_VM() {


	Directory* root = new Directory();

	auto* dir = root->createDir("dir");
	auto* subdir = dir->createDir("subdir");
	auto* ssf = subdir->createFile("subsubfile");

	delete dir;

	File* file = root->createFile("ahoj");
	FileHandle* fh = new FileHandle(file);
	size_t written = 0;

	fh->writeFile("test", 4, written);
	fh->closeFile();


	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	Initialize_Kernel();

	//spustime shell - v realnem OS bychom ovsem spousteli login
	TEntryPoint shell = (TEntryPoint)GetProcAddress(User_Programs, "shell");
	if (shell) {
		CONTEXT regs;  //ted je regs jenom nejak vyplneno kvuli preakladci
		GetThreadContext(GetCurrentThread(), &regs);  //ale pak bude jeden z registru ukazovat na nejaky startup info blok
		shell(regs);
	}

	Shutdown_Kernel();
}