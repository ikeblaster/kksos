#include "io.h"
#include "kernel.h"
#include "filesystem_utils.h"

using namespace FileSystem;

void HandleIO(CONTEXT &regs) {


	switch (Get_AL((__int16) regs.Rax)) {
		case scCreateFile:
			{
				IHandle* fh = FileSystem::Utils::CreateHandle(fs_cwd, (char*) regs.Rdx, (size_t) regs.Rcx);
				regs.Rax = (decltype(regs.Rax)) fh;

				// regs.Rax = (decltype(regs.Rax))CreateFileA((char*)regs.Rdx, GENERIC_READ | GENERIC_WRITE, (DWORD)regs.Rcx, 0, OPEN_EXISTING, 0, 0);
				// zde je treba podle Rcx doresit shared_read, shared_write, OPEN_EXISING, etc. podle potreby
				// Set_Error(fh == nullptr, regs);
			}
			break;


		case scWriteFile:
			{
				size_t written;
				IHandle* fh = (IHandle*) regs.Rdx;
				fh->write((void*) regs.Rdi, regs.Rcx, &written);
				regs.Rax = (DWORD) written;
				//Set_Error(false, regs);
			}
			break;

		case scReadFile:
			{
				size_t read = 0;
				IHandle* fh = (IHandle*) regs.Rdx;
				fh->read((char**) regs.Rdi, regs.Rcx, &read);
				regs.Rax = (DWORD) read;
				//Set_Error(false, regs);
			}
			break;

		case scSeekFile:
			{
				size_t read = 0;
				IHandle* fh = (IHandle*) regs.Rdx;
				regs.Rax = (DWORD) fh->seek((const fpos_t) regs.Rdi, (std::ios_base::seekdir) regs.Rcx);
				//Set_Error(false, regs);
			}
			break;

		case scCloseFile:
			{
				IHandle* fh = (IHandle*) regs.Rdx;
				fh->close();
				//Set_Error(!CloseHandle((HANDLE)regs.Rdx), regs);
			}
			break;

	}
}