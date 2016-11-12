#pragma once
#include "process.h"


namespace Process
{
	std::mutex table_mtx;
	PCB* table[PROCESS_TABLE_SIZE] = { nullptr };
	thread_local PCB* current_thread_pcb = nullptr;

	void program_thread(TEntryPoint program, PCB* pcb) {

		current_thread_pcb = pcb;

		CONTEXT regs;
		regs.Rcx = (decltype(regs.Rcx)) &pcb->psi;
		program(regs); // TODO: nepredavat 

		notify_handles_exit(pcb->psi);
	}

	pid_t create_process(PROCESSSTARTUPINFO psi) {
		TEntryPoint program = (TEntryPoint) GetProcAddress(User_Programs, psi.process_name.c_str());

		if (!program)
			return -1; // TODO: errors enum + osetreni na vyssi urovni

		std::unique_lock<std::mutex> lck(table_mtx);

		pid_t pid = get_free_spot_in_TT();

		if (pid > 0) {
			PCB* pcb = new PCB();

			table[pid] = pcb;

			pcb->psi = psi;
			pcb->pid = pid;

			if (current_thread_pcb != nullptr) {
				pcb->current_dir = current_thread_pcb->current_dir;
				pcb->ppid = current_thread_pcb->pid;
			}
			else {
				pcb->current_dir = FileSystem::fs_root;
				pcb->ppid = -1;
			}

			pcb->thread = new std::thread(program_thread, program, pcb);

		}
		else {
			notify_handles_exit(psi);
		}

		return pid;
	}

	bool join_process(pid_t pid) {
		if (pid <= 0 || pid >= PROCESS_TABLE_SIZE || table[pid] == nullptr)
			return false;

		table[pid]->thread->join();

		std::unique_lock<std::mutex> lck(table_mtx);

		delete table[pid]->thread;
		delete table[pid];

		table[pid] = nullptr;

		return true;
	}

	THandle get_std_handle(DWORD nStdHandle) {
		switch (nStdHandle) {
			case IHANDLE_STDIN: return Process::current_thread_pcb->psi.p_stdin;
			case IHANDLE_STDOUT: return Process::current_thread_pcb->psi.p_stdout;
			case IHANDLE_STDERR: return Process::current_thread_pcb->psi.p_stderr;
		}
		return nullptr;
	}

	void set_std_handle(DWORD nStdHandle, THandle handle) {
		switch (nStdHandle) {
			case IHANDLE_STDIN: Process::current_thread_pcb->psi.p_stdin = handle;
			case IHANDLE_STDOUT: Process::current_thread_pcb->psi.p_stdout = handle;
			case IHANDLE_STDERR: Process::current_thread_pcb->psi.p_stderr = handle;
		}
	}

	std::string get_cwd(pid_t pid) {
		PCB* pcb = nullptr;
		if (pid == 0) pcb = current_thread_pcb;
		if (pid > 0 && pid < PROCESS_TABLE_SIZE) pcb = table[pid];
		if (pcb == nullptr) return nullptr;

		return FileSystem::Path::generate(pcb->current_dir);
	}

	bool set_cwd(std::string path, pid_t pid) {
		PCB* pcb = nullptr;
		if (pid == 0) pcb = current_thread_pcb;
		if (pid > 0 && pid < PROCESS_TABLE_SIZE) pcb = table[pid];
		if (pcb == nullptr) return false;

		FileSystem::Directory* dir;
		FileSystem::File* file;

		auto res = FileSystem::Path::parse(pcb->current_dir, path, &dir, &file);
		if (res != FileSystem::RESULT::OK) 
			return false;
		
		pcb->current_dir = dir;

		return true;
	}

	void notify_handles_exit(PROCESSSTARTUPINFO &psi) {
		((FileSystem::IHandle*) psi.p_stdout)->close();
		((FileSystem::IHandle*) psi.p_stdin)->close();
		((FileSystem::IHandle*) psi.p_stderr)->close();
	}

	pid_t get_free_spot_in_TT() {
		for (int i = 1; i < PROCESS_TABLE_SIZE; i++) {
			if (table[i] == nullptr) {
				return i;
			}
		}
		return -1; // no free slot in task table
	}
}



void HandleProcess(CONTEXT &regs) {
	switch (Get_AL((__int16) regs.Rax)) {

		case scCreateProcess:
			regs.Rax = (decltype(regs.Rax)) Process::create_process(*(PROCESSSTARTUPINFO*) regs.Rcx);
			break;

		case scJoinProcess:
			regs.Rax = (decltype(regs.Rax)) Process::join_process((int) regs.Rdx);
			break;

		case scGetStdHandle:
			regs.Rax = (decltype(regs.Rax)) Process::get_std_handle((DWORD) regs.Rdx);
			break;

		case scSetStdHandle:
			Process::set_std_handle((DWORD) regs.Rdx, (THandle) regs.Rcx);
			break;

		case scGetCwd:
			regs.Rax = (decltype(regs.Rax)) new std::string(Process::get_cwd((int) regs.Rdx)); // must be on heap
			break;

		case scSetCwd:
			regs.Rax = (decltype(regs.Rax)) Process::set_cwd(*(std::string*) regs.Rcx, (int) regs.Rdx);
			break;

	}
}