#pragma once
#include <atomic>
#include <memory>
#include <sstream> 
#include <vector>
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
int Create_Process(
	std::string process_name, 
	std::vector<char> params, std::vector<std::string> data, 
	const THandle hstdin, const THandle hstdout, const THandle hstderr
);

/// <summary>
/// Waits for process to end.
/// </summary>
/// <param name="PID">PID of process.</param>
/// <returns>Success.</returns>
bool Join_Process(int PID);

/// <summary>
/// Gets handle to standard stream.
/// </summary>
/// <param name="nStdHandle">Stream type.</param>
/// <returns>Handle.</returns>
THandle Get_Std_Handle(DWORD nStdHandle);

/// <summary>
/// Sets standard stream to different handle.
/// </summary>
/// <param name="n_handle">Stream type.</param>
/// <param name="handle">New handle.</param>
void Set_Std_Handle(DWORD n_handle, THandle handle);

std::string Get_Cwd(int pid = -1);

bool Set_Cwd(std::string path, int pid = -1);

// =================================== FILES ===================================


/// <summary>
/// Opens/creates file (according to flags).
/// </summary>
/// <param name="file_name">Path to file.</param>
/// <param name="flags">Flags.</param>
/// <returns>Non-<c>nullptr</c> file handle on success.</returns>
THandle Create_File(const char* file_name, size_t flags);

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
