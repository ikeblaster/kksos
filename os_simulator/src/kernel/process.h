#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <thread>
#include "..\common\api.h"
#include "kernel.h"
#include "filesystem.h"
#include "filesystem.ihandle.h"

namespace Process {

	typedef struct PCB
	{
		int pid;
		std::thread* thread;
		PROCESSSTARTUPINFO psi;
		FileSystem::Directory* current_dir;

		// TODO: vector<> opened_files/handles

	} PCB;

	const size_t PROCESS_TABLE_SIZE = 1024;

	extern PCB* table[];
	extern thread_local PCB* current_thread_pcb;

	int create_process(PROCESSSTARTUPINFO psi);
	bool join_process(int pid);
	void notify_handles_exit(PROCESSSTARTUPINFO &psi);
	std::string get_cwd(int pid = -1);
	bool set_cwd(std::string path, int pid = -1);
	int get_free_spot_in_TT();
}

void HandleProcess(CONTEXT &regs);
