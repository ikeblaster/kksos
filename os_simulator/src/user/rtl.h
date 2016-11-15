#pragma once
#include <atomic>
#include <memory>
#include "..\common\api.h"

size_t Get_Last_Error();


// ================================= PROCESSES =================================

/// <summary>
/// Creates new process.
/// </summary>
/// <param name="process_name">Name of program to run (future process name).</param>
/// <param name="params">Program parameters.</param>
/// <param name="data">Program data parameters.</param>
/// <param name="hstdin">Handle to standard input stream.</param>
/// <param name="hstdout">Handle to standard output stream.</param>
/// <param name="hstderr">Handle to standard error stream.</param>
/// <returns>PID of created process or error code.</returns>
pid_t Create_Process(
	std::string process_name,
	std::vector<char> params, std::vector<std::string> data,
	const THandle hstdin, const THandle hstdout, const THandle hstderr
);

/// <summary>
/// Waits for process to end.
/// </summary>
/// <param name="PID">PID of process.</param>
/// <returns>Success.</returns>
bool Join_Process(pid_t PID);

/// <summary>
/// Gets current working directory path.
/// </summary>
/// <param name="pid">PID of desired process; -1 for current process.</param>
/// <returns>Path.</returns>
std::string Get_Cwd();

/// <summary>
/// Sets current working directory.
/// </summary>
/// <param name="path">Path.</param>
/// <param name="pid">PID of desired process; -1 for current process.</param>
/// <returns>Success.</returns>
bool Set_Cwd(std::string path);

/// <summary>
/// Gets listing of processes
/// </summary>
/// <param name="items">Processes</param>
void List_Processes(std::vector<std::string> &items);

/// <summary>
/// Checks whether any of running processes are using specified working directory
/// </summary>
/// <param name="path">Working directory</param>
/// <returns>There is a process using a specified directory</returns>
bool Check_Cwd(std::string path);


// =================================== IO ===================================

/// <summary>
/// Opens/creates file (according to flags).
/// </summary>
/// <param name="file_name">Path to file.</param>
/// <param name="flags">Flags.</param>
/// <returns>Non-<c>nullptr</c> file handle on success.</returns>
THandle Create_File(const char* file_name, flags_t flags);

/// <summary>
/// Writes data to file handle.
/// </summary>
/// <param name="file_handle">File handle.</param>
/// <param name="buffer">Buffer with data.</param>
/// <param name="buffer_size">Amount of data in buffer.</param>
/// <param name="written">[out] Amount of data successfully written.</param>
/// <returns>Success.</returns>
bool Write_File(const THandle file_handle, const void *buffer, const size_t buffer_size, size_t &written);

/// <summary>
/// Reads data from file handle.
/// </summary>
/// <param name="file_handle">File handle.</param>
/// <param name="buffer">[out] Allocated buffer for data.</param>
/// <param name="buffer_size">Buffer size.</param>
/// <param name="read">[out] Amount of data successfully read.</param>
/// <returns>Success.</returns>
bool Read_File(const THandle file_handle, const void **buffer, const size_t buffer_size, size_t &read);

/// <summary>
/// Changes position in file handle.
/// Special case: for <c>Pipe</c> setting <c>way</c> to <c>beg+end</c> marks the pipe as closed.
/// </summary>
/// <param name="file_handle">File handle</param>
/// <param name="pos">New position.</param>
/// <param name="way">Direction type.</param>
/// <returns>New position.</returns>
fpos_t Seek_File(const THandle file_handle, const fpos_t pos, std::ios_base::seekdir way);

/// <summary>
/// Closes+destructs file handle.
/// </summary>
/// <param name="file_handle">File handle.</param>
/// <returns>Success.</returns>
bool Close_File(const THandle file_handle);

/// <summary>
/// Creates pipe.
/// </summary>
/// <param name="readable">[out] Handle for readable end of pipe.</param>
/// <param name="writeable">[out] Handle for writeable end of pipe.</param>
void Create_Pipe(THandle &readable, THandle &writeable);

/// <summary>
/// Creates a directory at specified location
/// </summary>
/// <param name="path">Path to the directory</param>
/// <returns></returns>
bool Make_Directory(std::string path);

/// <summary>
/// Removes a directory at specified location
/// </summary>
/// <param name="path">Path to the directory</param>
/// <returns></returns>
bool Remove_Directory(std::string path);


/// <summary>
/// Gets listing of directory
/// </summary>
/// <param name="items">Items</param>
void List_Directory(std::vector<std::string> &items);
