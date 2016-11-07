#pragma once
#include "process.h"


namespace  Process 
{
	int create_process(p_params *params) {
		PCB* pcb = new PCB();

		pcb->PID = get_free_spot_in_TT();

		table[pcb->PID] = pcb;

		//TODO Doplnit v�echny z�znamy PCB

		return pcb->PID;

		//TODO o�et�it n�vratovou hodnotu... n�kde...... n�jak.... pros�m 
	}

	int get_free_spot_in_TT() {

		for (int i = 0; i < PROCESS_TABLE_SIZE; i++) {
			if (table[i] == nullptr) return i;
		}

		return -1; //No free spots in task table
	}
}
