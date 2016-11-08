#pragma once
#include "process.h"


namespace Process
{
	const PCB* table[PROCESS_TABLE_SIZE] = { nullptr };

	void thread_something(PCB* pcb) {
		TEntryPoint program = (TEntryPoint)GetProcAddress(User_Programs, pcb->psi.process_name.c_str());
		if (program) {
			CONTEXT regs; // ted je regs jenom nejak vyplneno kvuli preakladci
			regs.Rcx = (decltype(regs.Rcx))&pcb->psi;
			program(regs);
		}
	}

	int create_process(PROCESSSTARTUPINFO psi) {
		int pid = get_free_spot_in_TT();
		if (pid >= 0) {

			PCB* pcb = new PCB();

			pcb->pid = pid;
			pcb->psi = psi;

			table[pcb->pid] = pcb;

			std::thread* thr = new std::thread(thread_something, pcb);
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
		return -1; //No free spots in task table
	}
}



void HandleProcess(CONTEXT &regs) {


	switch (Get_AL((__int16)regs.Rax)) {
		case scCreateProcess:
		{
			regs.Rax = (decltype(regs.Rax))Process::create_process(*(PROCESSSTARTUPINFO*) regs.Rcx);
		}
		break;


		case scJoinProcess:
		{
			regs.Rax = (decltype(regs.Rax))Process::join_process((int)regs.Rdx);
			//Set_Error(false, regs);

			//const bool failed = !WriteFile((HANDLE)regs.Rdx, (void*)regs.Rdi, (DWORD)regs.Rcx, &written, NULL);

		}
		break;
	}
}