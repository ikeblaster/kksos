#pragma once
#include "process.h"


namespace Process
{
	const PCB* table[PROCESS_TABLE_SIZE] = { nullptr };
	thread_local PCB* current_thread_pcb;

	void program_thread(TEntryPoint program, PCB* pcb) {

		current_thread_pcb = pcb;

		CONTEXT regs; // ted je regs jenom nejak vyplneno kvuli prekladaci
		regs.Rcx = (decltype(regs.Rcx)) &pcb->psi;
		program(regs);

		((FileSystem::IHandle*) pcb->psi.p_stdout)->seek(0, std::ios_base::beg + std::ios_base::end);
		((FileSystem::IHandle*) pcb->psi.p_stdin)->seek(0, std::ios_base::beg + std::ios_base::end);
		((FileSystem::IHandle*) pcb->psi.p_stderr)->seek(0, std::ios_base::beg + std::ios_base::end);
	}

	int create_process(PROCESSSTARTUPINFO psi) {
		TEntryPoint program = (TEntryPoint) GetProcAddress(User_Programs, psi.process_name.c_str());

		if (!program)
			return -1; // TODO: errors enum

		int pid = get_free_spot_in_TT();
		if (pid >= 0) {

			PCB* pcb = new PCB();

			pcb->pid = pid;
			pcb->psi = psi;

			table[pcb->pid] = pcb;


			std::thread* thr = new std::thread(program_thread, program, pcb);
			pcb->thread = thr;
		}

		return pid;
	}

	bool join_process(int pid) {
		if (table[pid] == nullptr)
			return false;

		table[pid]->thread->join();
		return true;
	}

	int get_free_spot_in_TT() {
		for (int i = 0; i < PROCESS_TABLE_SIZE; i++) {
			if (table[i] == nullptr) return i;
		}
		return -1; // no free slot in task table
	}

	THandle get_std_handle(DWORD nStdHandle) {
		switch (nStdHandle) {
			case IHANDLE_STDIN: return Process::current_thread_pcb->psi.p_stdin;
			case IHANDLE_STDOUT: return Process::current_thread_pcb->psi.p_stdout;
			case IHANDLE_STDERR: return Process::current_thread_pcb->psi.p_stderr;
		}
		return nullptr;
	}
}



void HandleProcess(CONTEXT &regs) {

	switch (Get_AL((__int16) regs.Rax)) {
		case scCreateProcess:
			{
				regs.Rax = (decltype(regs.Rax)) Process::create_process(*(PROCESSSTARTUPINFO*) regs.Rcx);
			}
			break;

		case scJoinProcess:
			{
				regs.Rax = (decltype(regs.Rax)) Process::join_process((int) regs.Rdx);
				//Set_Error(false, regs);

				//const bool failed = !WriteFile((HANDLE)regs.Rdx, (void*)regs.Rdi, (DWORD)regs.Rcx, &written, NULL);

			}
			break;

		case scGetStdHandle:
			{
				regs.Rax = (decltype(regs.Rax)) Process::get_std_handle((DWORD) regs.Rdx);
			}
			break;
	}
}