#pragma once
#include "process.h"


namespace Process
{
	std::mutex table_mtx, handles_mtx;
	PCB* table[PROCESS_TABLE_SIZE] = { nullptr };
	OpenFiles::OFHandle file_descriptors[FILE_DESCRIPTORS_TABLE_SIZE] = { nullptr };
	thread_local PCB* current_thread_pcb = nullptr;

	void program_thread(TEntryPoint program, PCB* pcb) {

		current_thread_pcb = pcb;

		CONTEXT regs;
		regs.Rcx = (decltype(regs.Rcx)) &pcb->psi;
		program(regs); // TODO: nepredavat 

		free_handles();
	}

	pid_t create_process(PROCESSSTARTUPINFO psi) {
		TEntryPoint program = (TEntryPoint) GetProcAddress(User_Programs, psi.process_name.c_str());

		if (!program)
			return -1; // TODO: errors enum + osetreni na vyssi urovni

		std::unique_lock<std::mutex> lck(table_mtx);

		pid_t pid = -1;
		for (int i = 0; i < PROCESS_TABLE_SIZE; i++) {
			if (table[i] == nullptr) {
				pid = i; // found free slot
				break;
			}
		}

		if (pid >= 0) {
			PCB* pcb = new PCB();
			table[pid] = pcb;

			pcb->pid = pid;
			pcb->psi = psi;

			if (current_thread_pcb != nullptr) {
				pcb->ppid = current_thread_pcb->pid;
				pcb->current_dir = current_thread_pcb->current_dir;

				set_handle(pcb, (THandle) IHANDLE_STDIN, get_handle(psi.h_stdin));
				set_handle(pcb, (THandle) IHANDLE_STDOUT, get_handle(psi.h_stdout));
				set_handle(pcb, (THandle) IHANDLE_STDERR, get_handle(psi.h_stderr));
 
				// => move file descriptors to new process except stdin/out/err (which will handle parent at it's end)
	/*			if (psi.h_stdin != (THandle) IHANDLE_STDIN) close_handle(psi.h_stdin);
				if (psi.h_stdout != (THandle) IHANDLE_STDOUT) close_handle(psi.h_stdout);
				if (psi.h_stderr != (THandle) IHANDLE_STDERR) close_handle(psi.h_stderr);*/
			}
			else {
				pcb->ppid = -1;
				pcb->current_dir = FileSystem::fs_root;

				set_handle(pcb, (THandle) IHANDLE_STDIN, FileSystem::console);
				set_handle(pcb, (THandle) IHANDLE_STDOUT, FileSystem::console);
				set_handle(pcb, (THandle) IHANDLE_STDERR, FileSystem::console);
			}

			pcb->thread = new std::thread(program_thread, program, pcb);
		}
		else if(current_thread_pcb != nullptr) {
			close_handle(psi.h_stdin);
			close_handle(psi.h_stdout);
			close_handle(psi.h_stderr);
		}

		return pid;
	}

	bool join_process(pid_t pid) {
		if (pid < 0 || pid >= PROCESS_TABLE_SIZE || table[pid] == nullptr)
			return false;

		table[pid]->thread->join();

		std::unique_lock<std::mutex> lck(table_mtx);

		delete table[pid]->thread;
		delete table[pid]; // delete pcb

		table[pid] = nullptr;

		return true;
	}

	THandle get_std_handle(THandle nStdHandle) {
		return nStdHandle;
	}

	void set_std_handle(DWORD nStdHandle, THandle handle) {
		// TODO: delete
	}

	std::string get_cwd() {
		return FileSystem::Path::generate(current_thread_pcb->current_dir); // TODO: vracet pres char** parameter
	}

	bool set_cwd(std::string path) {
		FileSystem::Directory* dir;
		FileSystem::File* file;

		auto res = FileSystem::Path::parse(current_thread_pcb->current_dir, path, &dir, &file);
		if (res != FileSystem::RESULT::OK)
			return false;

		current_thread_pcb->current_dir = dir;
		return true;
	}

	FileSystem::FSHandle* get_handle(THandle fd) {
		std::unique_lock<std::mutex> lck(handles_mtx);

		if (fd < 0 || (intptr_t) fd > FILE_DESCRIPTORS_TABLE_SIZE) 
			return nullptr;

		OpenFiles::OFHandle of = current_thread_pcb->file_descriptors[(intptr_t) fd];
		return OpenFiles::GetFSHandle(of);
	}


	bool set_handle(PCB* pcb, THandle fd, FileSystem::FSHandle* handle) {
		std::unique_lock<std::mutex> lck(handles_mtx);

		auto of = OpenFiles::CreateHandle(handle);
		if (of == nullptr) return false;
		pcb->file_descriptors[(intptr_t) fd] = of;
		return true;
	}

	THandle add_handle(FileSystem::FSHandle* handle) {
		std::unique_lock<std::mutex> lck(handles_mtx);

		for (intptr_t i = 0; i < FILE_DESCRIPTORS_TABLE_SIZE; i++) {
			if (current_thread_pcb->file_descriptors[i] == nullptr) { // found free slot

				auto of = OpenFiles::CreateHandle(handle);
				if (of == nullptr)
					break;

				current_thread_pcb->file_descriptors[i] = of;

				return (THandle) i;
			}
		}
		return (THandle) -1;
	}

	bool close_handle(THandle fd) {
		std::unique_lock<std::mutex> lck(handles_mtx);

		OpenFiles::OFHandle of = current_thread_pcb->file_descriptors[(intptr_t) fd];
		current_thread_pcb->file_descriptors[(intptr_t) fd] = nullptr;

		return OpenFiles::CloseHandle(of);
	}

	void free_handles() {
		std::unique_lock<std::mutex> lck(handles_mtx);

		for (intptr_t i = 0; i < FILE_DESCRIPTORS_TABLE_SIZE; i++) {
			if (current_thread_pcb->file_descriptors[i] != nullptr) {

				OpenFiles::CloseHandle(current_thread_pcb->file_descriptors[i]);
				current_thread_pcb->file_descriptors[i] = nullptr;

			}
		}
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
			regs.Rax = (decltype(regs.Rax)) Process::get_std_handle((THandle) regs.Rdx);
			break;

		case scSetStdHandle:
			Process::set_std_handle((DWORD) regs.Rdx, (THandle) regs.Rcx);
			break;

		case scGetCwd:
			regs.Rax = (decltype(regs.Rax)) new std::string(Process::get_cwd());
			break;

		case scSetCwd:
			regs.Rax = (decltype(regs.Rax)) Process::set_cwd(*(std::string*) regs.Rcx);
			break;

	}
}