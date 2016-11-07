#pragma once

#include "filesystem.h"
#include <Windows.h>
#include <string>
#include "..\common\api.h"

namespace Process {

	const size_t PROCESS_TABLE_SIZE = 1024;

	/*
	PCB - Process Control Block

	Obsahuje informace o procesu

	current_dir - adresar ze ktereho je proces spusten
	PID - jedinecny identifikator procesu
	name - nazev procesu
	context - kontext procesu

	p_stdin, p_stdout, p_stderr - I/O rozhrani procesu

	*/
	typedef struct PCB
	{
		FileSystem::Directory* current_dir;

		int PID;

		std::string process_name; // ?

		CONTEXT context;

		THandle p_stdin;
		THandle p_stdout;
		THandle p_stderr;

	} PCB;

	const PCB* table[PROCESS_TABLE_SIZE] = { nullptr };

	//Jen navrh
	typedef struct p_params {
		char ** argv;
		int argc;
	} p_params;


	int create_process(p_params *params);
	int get_free_spot_in_TT();

}
