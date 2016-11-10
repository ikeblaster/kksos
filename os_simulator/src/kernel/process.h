#pragma once
#include <string>
#include <mutex>
#include <thread>
#include "..\common\api.h"
#include "kernel.h"
#include "filesystem.h"
#include "filesystem.ihandle.h"


namespace Process {

	typedef struct PCB
	{
		int ppid;
		int pid;
		std::thread* thread;
		PROCESSSTARTUPINFO psi;
		FileSystem::Directory* current_dir;

		// TODO: vector<> opened_files/handles

	} PCB;

	const size_t PROCESS_TABLE_SIZE = 1024;

	extern std::mutex table_mtx;
	extern PCB* table[];
	extern thread_local PCB* current_thread_pcb;

	int create_process(PROCESSSTARTUPINFO psi);
	bool join_process(int pid);
	void notify_handles_exit(PROCESSSTARTUPINFO &psi);
	std::string get_cwd(int pid = 0);
	bool set_cwd(std::string path, int pid = 0);
	int get_free_spot_in_TT();

}

void HandleProcess(CONTEXT &regs);
