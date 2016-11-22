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
	enum class State { New, Running, Terminated };

	const size_t PROCESS_TABLE_SIZE = 1024;
	const size_t FILE_DESCRIPTORS_TABLE_SIZE = 1024;

	struct PCB
	{
		pid_t ppid; // parent process ID
		pid_t pid; // process ID
		std::thread* thread; // thread pointer
		PROCESSSTARTUPINFO psi; // startup data
		FileSystem::Directory* current_dir; // current directory
		OpenFiles::OFHandle file_descriptors[FILE_DESCRIPTORS_TABLE_SIZE]; // table of file descriptors
		State state; // process state
		size_t exit_code; // exit code (only when process is in terminated state)
	};


	PCB* table[];
	extern thread_local PCB* current_thread_pcb;

	/// <summary>
	/// Entrypoint for new processes, calls entrypoint of a program.
	/// </summary>
	/// <param name="program">Entrypoint of a program.</param>
	/// <param name="pcb"></param>
	void program_thread(TEntryPoint program, PCB* pcb);

	/// <summary>
	/// Creates a new process (started at creation).
	/// </summary>
	/// <param name="psi">Startup info structure</param>
	/// <returns>PID of new process or <c>-1</c> as an error.</returns>
	pid_t create_process(PROCESSSTARTUPINFO* psi);

	/// <summary>
	/// Waits for a process to end.
	/// </summary>
	/// <param name="pid">PID of the process.</param>
	/// <returns><c>False</c> if process not found.</returns>
	bool join_process(pid_t pid);

	/// <summary>
	/// Lists all processes into a vector.
	/// </summary>
	/// <param name="items">[out] Vector of strings.</param>
	void list_processes(std::vector<std::string>* items);

	/// <summary>
	/// Gets current working directory.
	/// </summary>
	/// <returns>Path.</returns>
	std::string get_cwd();

	/// <summary>
	/// Sets new current working directory.
	/// </summary>
	/// <param name="path">Path.</param>
	/// <returns>Success.</returns>
	bool set_cwd(std::string path);


	// private functions

	/// <summary>
	/// Gets <c>FSHandle</c> from file descriptor (in current process).
	/// </summary>
	/// <param name="fd">Handle-file descriptor.</param>
	/// <returns><c>FSHandle</c></returns>
	FileSystem::FSHandle* get_handle(THandle fd);

	/// <summary>
	/// Sets new <c>FSHandle</c> as specified file descriptor.
	/// Closes any previously opened handles on the same FD.
	/// </summary>
	/// <param name="pcb">PCB of destination process.</param>
	/// <param name="fd">File descriptor.</param>
	/// <param name="handle">New <c>FSHandle</c> to be set.</param>
	/// <returns>Success.</returns>
	bool set_handle(PCB* pcb, THandle fd, FileSystem::FSHandle* handle);

	/// <summary>
	/// Adds new <c>FSHandle</c> as new file descriptor (to current process).
	/// </summary>
	/// <param name="handle">New <c>FSHandle</c> to be added.</param>
	/// <returns>Added file descriptor.</returns>
	THandle add_handle(FileSystem::FSHandle* handle);

	/// <summary>
	/// Closes handle specified by file descriptor.
	/// </summary>
	/// <param name="fd">File descriptor of a handle to be closed.</param>
	/// <returns>Success.</returns>
	bool close_handle(THandle fd);

	/// <summary>
	/// Closes all unclosed handles (of current process).
	/// </summary>
	void free_handles();

}

void HandleProcess(CONTEXT &regs);
