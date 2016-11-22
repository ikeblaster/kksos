#pragma once
#include "process.h"


namespace Process
{
	std::mutex table_mtx, handles_mtx;
	PCB* table[PROCESS_TABLE_SIZE] = { nullptr };
	thread_local PCB* current_thread_pcb = nullptr;

	void program_thread(TEntryPoint program, PCB* pcb)
	{
		current_thread_pcb = pcb;
		pcb->state = State::Running;

		CONTEXT regs;
		regs.Rcx = (decltype(regs.Rcx)) const_cast<const std::vector<std::string>*>(&(pcb->psi.params));
		pcb->exit_code = program(regs);

		free_handles();
		pcb->state = State::Terminated;
	}

	pid_t create_process(PROCESSSTARTUPINFO* psi)
	{
		TEntryPoint program;

		if (psi->subprocess_entry != nullptr)
			program = psi->subprocess_entry;
		else
			program = (TEntryPoint) GetProcAddress(User_Programs, psi->process_name.c_str());

		if (!program)
			return -1;

		std::unique_lock<std::mutex> lck(table_mtx);

		pid_t pid = -1;
		for (int i = 0; i < PROCESS_TABLE_SIZE; i++) {
			if (table[i] == nullptr) {
				pid = i; // found free slot
				break;
			}
		}

		if (pid != -1) {
			PCB* pcb = new PCB();
			table[pid] = pcb;

			pcb->pid = pid;
			pcb->psi = std::move(*psi); // move into PCB
			pcb->state = State::New;

			if (current_thread_pcb != nullptr) {
				pcb->ppid = current_thread_pcb->pid;
				pcb->current_dir = current_thread_pcb->current_dir;

				set_handle(pcb, THANDLE_STDIN, get_handle(psi->h_stdin));
				set_handle(pcb, THANDLE_STDOUT, get_handle(psi->h_stdout));
				set_handle(pcb, THANDLE_STDERR, get_handle(psi->h_stderr));
			}
			else {
				pcb->ppid = -1;
				pcb->current_dir = FileSystem::fs_root;

				set_handle(pcb, THANDLE_STDIN, FileSystem::console);
				set_handle(pcb, THANDLE_STDOUT, FileSystem::console);
				set_handle(pcb, THANDLE_STDERR, FileSystem::console);
			}

			pcb->thread = new std::thread(program_thread, program, pcb);
		}

		return pid;
	}

	bool join_process(pid_t pid) 
	{
		if (pid < 0 || pid >= PROCESS_TABLE_SIZE || table[pid] == nullptr)
			return false;

		table[pid]->thread->join();

		std::unique_lock<std::mutex> lck(table_mtx);

		delete table[pid]->thread;
		delete table[pid]; // delete pcb

		table[pid] = nullptr;

		return true;
	}

	void list_processes(std::vector<std::string>* items)
	{
		std::unique_lock<std::mutex> lck(table_mtx);

		for (int i = 0; i < PROCESS_TABLE_SIZE; i++) {
			if (table[i] != nullptr) {
				State state = table[i]->state;
				items->push_back(
					table[i]->psi.process_name +
					"\tPID: " + std::to_string(table[i]->pid) +
					"\tPPID: " + (table[i]->ppid == -1 ? "-" : std::to_string(table[i]->ppid)) +
					"\tState: " + (state == State::Terminated ? "Terminated" : state == State::Running ? "Running" : "New")
				);
			}
		}
	}

	std::string get_cwd() 
	{
		return FileSystem::Path::generate(current_thread_pcb->current_dir);
	}

	bool set_cwd(std::string path) 
	{
		FileSystem::Directory* dir;
		FileSystem::File* file;

		auto res = FileSystem::Path::parse(current_thread_pcb->current_dir, path, &dir, &file, 0);
		if (res != FileSystem::RESULT::OK)
			return false;

		current_thread_pcb->current_dir = dir;
		return true;
	}



	// private functions
	FileSystem::FSHandle* get_handle(THandle fd)
	{
		std::unique_lock<std::mutex> lck(handles_mtx);

		if (fd < 0 || (intptr_t) fd > FILE_DESCRIPTORS_TABLE_SIZE)
			return nullptr;

		OpenFiles::OFHandle of = current_thread_pcb->file_descriptors[(intptr_t) fd];
		return OpenFiles::GetFSHandle(of);
	}

	bool set_handle(PCB* pcb, THandle fd, FileSystem::FSHandle* handle) 
	{
		std::unique_lock<std::mutex> lck(handles_mtx);

		auto of = OpenFiles::CreateHandle(handle);
		if (of == nullptr) 
			return false;

		if (pcb->file_descriptors[(intptr_t) fd] != nullptr)
			close_handle(fd); // close existing handle

		pcb->file_descriptors[(intptr_t) fd] = of;
		return true;
	}

	THandle add_handle(FileSystem::FSHandle* handle) 
	{
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
		return INVALID_THANDLE;
	}

	bool close_handle(THandle fd) 
	{
		if (fd < 0 || (size_t) fd > Process::FILE_DESCRIPTORS_TABLE_SIZE)
			return false;

		std::unique_lock<std::mutex> lck(handles_mtx);

		OpenFiles::OFHandle of = current_thread_pcb->file_descriptors[(intptr_t) fd];
		current_thread_pcb->file_descriptors[(intptr_t) fd] = nullptr;

		return OpenFiles::CloseHandle(of);
	}

	void free_handles() 
	{
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
			regs.Rax = (decltype(regs.Rax)) Process::create_process((PROCESSSTARTUPINFO*) regs.Rcx);
			Set_Error(regs.Rax == -1, regs);
			break;

		case scJoinProcess:
			Set_Error(!Process::join_process((int) regs.Rcx), regs);
			break;

		case scGetCwd:
			((std::string*) regs.Rcx)->assign(Process::get_cwd());
			break;

		case scSetCwd:
			Set_Error(!Process::set_cwd(*(std::string*) regs.Rcx), regs);
			break;

		case scListProcesses:
			Process::list_processes((std::vector<std::string>*) regs.Rdx);
			break;

	}
}