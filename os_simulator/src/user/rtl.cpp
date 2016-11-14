#include "rtl.h"

extern "C" __declspec(dllimport) void __stdcall SysCall(CONTEXT &context);


std::atomic<size_t> LastError;

size_t Get_Last_Error()
{
	return LastError;
}

CONTEXT Prepare_SysCall_Context(__int8 major, __int8 minor)
{
	CONTEXT regs;
	regs.Rax = Compose_AX(major, minor);
	return regs;
}

bool Do_SysCall(CONTEXT &regs)
{
	SysCall(regs);

	const bool failed = test_cf(regs.EFlags);
	if (failed) LastError = regs.Rax;
	else LastError = 0;

	return !failed;
}


pid_t Create_Process(std::string process_name, std::vector<char> params, std::vector<std::string> data, const THandle hstdin, const THandle hstdout, const THandle hstderr)
{
	CONTEXT regs = Prepare_SysCall_Context(scProcess, scCreateProcess);

	PROCESSSTARTUPINFO psi;
	psi.process_name = process_name;
	psi.params = params;
	psi.data = data;
	psi.h_stdin = hstdin;
	psi.h_stdout = hstdout;
	psi.h_stderr = hstderr;

	regs.Rcx = (decltype(regs.Rcx)) &psi;

	Do_SysCall(regs);
	return (pid_t) regs.Rax;
}

bool Join_Process(pid_t pid) 
{
	CONTEXT regs = Prepare_SysCall_Context(scProcess, scJoinProcess);
	regs.Rdx = (decltype(regs.Rdx)) pid;
	Do_SysCall(regs);
	return regs.Rax != 0;
}

THandle Get_Std_Handle(DWORD64 n_handle)
{
	CONTEXT regs = Prepare_SysCall_Context(scProcess, scGetStdHandle);
	regs.Rdx = (decltype(regs.Rdx)) n_handle;
	Do_SysCall(regs);
	return (THandle) regs.Rax;
}

void Set_Std_Handle(DWORD64 n_handle, THandle handle)
{
	CONTEXT regs = Prepare_SysCall_Context(scProcess, scSetStdHandle);
	regs.Rdx = (decltype(regs.Rdx)) n_handle;
	regs.Rcx = (decltype(regs.Rcx)) handle;
	Do_SysCall(regs);
}

std::string Get_Cwd()
{
	CONTEXT regs = Prepare_SysCall_Context(scProcess, scGetCwd);
	Do_SysCall(regs);

	std::string* ret = (std::string*) regs.Rax;
	std::string path(*ret); // TODO: nejak lepe?
	delete ret;

	return path;
}

bool Set_Cwd(std::string path)
{
	CONTEXT regs = Prepare_SysCall_Context(scProcess, scSetCwd);
	regs.Rcx = (decltype(regs.Rcx)) &path;
	Do_SysCall(regs);
	return regs.Rax != 0;
}

void List_Directory(std::vector<std::string> items)
{
	CONTEXT regs = Prepare_SysCall_Context(scProcess, scListDirectory);
	regs.Rcx = (decltype(regs.Rcx)) &items;
	Do_SysCall(regs);
}




THandle Create_File(const char* file_name, flags_t flags)
{
	CONTEXT regs = Prepare_SysCall_Context(scIO, scCreateFile);
	regs.Rdx = (decltype(regs.Rdx)) file_name;
	regs.Rcx = (decltype(regs.Rcx)) flags;
	Do_SysCall(regs);
	return (THandle) regs.Rax;
}

bool Write_File(const THandle file_handle, const void *buffer, const size_t buffer_size, size_t &written)
{
	CONTEXT regs = Prepare_SysCall_Context(scIO, scWriteFile);
	regs.Rdx = (decltype(regs.Rdx)) file_handle;
	regs.Rdi = (decltype(regs.Rdi)) buffer;
	regs.Rcx = (decltype(regs.Rcx)) buffer_size;

	const bool result = Do_SysCall(regs);
	written = regs.Rax;
	return result;
}

bool Read_File(const THandle file_handle, const void **buffer, const size_t buffer_size, size_t &read)
{
	CONTEXT regs = Prepare_SysCall_Context(scIO, scReadFile);
	regs.Rdx = (decltype(regs.Rdx)) file_handle;
	regs.Rdi = (decltype(regs.Rdi)) buffer;
	regs.Rcx = (decltype(regs.Rcx)) buffer_size;

	const bool result = Do_SysCall(regs);
	read = regs.Rax;
	return result;
}

fpos_t Seek_File(const THandle file_handle, const fpos_t pos, std::ios_base::seekdir way)
{
	CONTEXT regs = Prepare_SysCall_Context(scIO, scSeekFile);
	regs.Rdx = (decltype(regs.Rdx)) file_handle;
	regs.Rdi = (decltype(regs.Rdi)) pos;
	regs.Rcx = (decltype(regs.Rcx)) way;

	Do_SysCall(regs);
	return regs.Rax;
}

bool Close_File(const THandle file_handle)
{
	CONTEXT regs = Prepare_SysCall_Context(scIO, scCloseFile);
	regs.Rdx = (decltype(regs.Rdx)) file_handle;
	return Do_SysCall(regs);
}

void Create_Pipe(THandle &readable, THandle &writeable)
{
	CONTEXT regs = Prepare_SysCall_Context(scIO, scCreatePipe);
	Do_SysCall(regs);
	readable = (THandle) regs.Rcx;
	writeable = (THandle) regs.Rdx;
}

bool Make_Directory(std::string path)
{
	CONTEXT regs = Prepare_SysCall_Context(scIO, scMakeDirectory);
	regs.Rdx = (decltype(regs.Rdx)) &path;
	Do_SysCall(regs);
	return (bool) regs.Rax;
}