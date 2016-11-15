#pragma once
#include <string>
#include <map>
#include <mutex>
#include <thread>
#include "..\common\api.h"
#include "kernel.h"
#include "filesystem.h"
#include "filesystem.fshandle.h"
#include "openfiles.h"


namespace Process 
{

	const size_t PROCESS_TABLE_SIZE = 1024;
	const size_t FILE_DESCRIPTORS_TABLE_SIZE = 1024;

	typedef struct PCB // TODO: vyzkouset jen struct PCB
	{
		pid_t ppid;
		pid_t pid;
		std::thread* thread;
		PROCESSSTARTUPINFO psi;
		FileSystem::Directory* current_dir;
		OpenFiles::OFHandle file_descriptors[FILE_DESCRIPTORS_TABLE_SIZE];

	} PCB;


	extern std::mutex table_mtx;
	extern PCB* table[];

	extern thread_local PCB* current_thread_pcb;

	pid_t create_process(PROCESSSTARTUPINFO psi);
	bool join_process(pid_t pid);
	std::string get_cwd();
	bool set_cwd(std::string path);

	FileSystem::FSHandle* get_handle(THandle fd);
	bool set_handle(PCB* pcb, THandle fd, FileSystem::FSHandle* handle);
	THandle add_handle(FileSystem::FSHandle* handle);
	bool close_handle(THandle fd);

	void free_handles();
	void list_processes(std::vector<std::string>* items);

}

void HandleProcess(CONTEXT &regs);
