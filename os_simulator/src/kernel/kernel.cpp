#include "kernel.h"

HMODULE User_Programs;

void Set_Error(const bool failed, CONTEXT &regs) {
	if (failed) {
		regs.EFlags = stc(regs.EFlags);
		// regs.Rax = GetLastError(); // only for winapi calls
	}
	else {
		regs.EFlags = clc(regs.EFlags);
	}
}

void SysCall(CONTEXT &regs) {

	switch (Get_AH((__int16) regs.Rax)) {
		case scIO:
			HandleIO(regs);
			break;
		case scProcess:
			HandleProcess(regs);
			break;
	}

}

void Initialize_Kernel() {
	User_Programs = LoadLibrary(L"user.dll");
}
void Shutdown_Kernel() {
	FreeLibrary(User_Programs);
}

void Initialize_FS() {
	FileSystem::fs = new FileSystem::Directory();
	FileSystem::fs_root = FileSystem::fs->createDirectory("C:");
	FileSystem::fs_root->createDirectory("slozka")->createDirectory("podslozka"); // TODO: smazat
	FileSystem::console = new FileSystem::ConsoleHandle();
}
void Shutdown_FS() {
	FileSystem::fs->destroy();
	delete FileSystem::console;
}


void Run_VM() {

	Initialize_Kernel();
	Initialize_FS();


	PROCESSSTARTUPINFO psi;
	psi.process_name = "shell";

	pid_t pid = Process::create_process(psi);
	Process::join_process(pid);


	Shutdown_FS();
	Shutdown_Kernel();

}

