#include "io.h"
#include "kernel.h"
#include "filesystem.filehandle.h"

using namespace FileSystem;

void HandleIO(CONTEXT &regs) {


	switch (Get_AL((__int16) regs.Rax)) {
		case scCreateFile: 
			{
				FileHandle* fh = FileHandle::CreateFileHandle(fs_cwd, (char*)regs.Rdx, (DWORD)regs.Rcx);
				regs.Rax = (decltype(regs.Rax))fh;
				Set_Error(fh == nullptr, regs);

				// regs.Rax = (decltype(regs.Rax))CreateFileA((char*)regs.Rdx, GENERIC_READ | GENERIC_WRITE, (DWORD)regs.Rcx, 0, OPEN_EXISTING, 0, 0);
				// zde je treba podle Rcx doresit shared_read, shared_write, OPEN_EXISING, etc. podle potreby
			}
			break;


		case scWriteFile:
			{
				size_t written;
				FileHandle* fh = (FileHandle*)regs.Rdx;
				fh->write((void*)regs.Rdi, regs.Rcx, &written);
				regs.Rax = (DWORD)written;

				Set_Error(false, regs);

				//const bool failed = !WriteFile((HANDLE)regs.Rdx, (void*)regs.Rdi, (DWORD)regs.Rcx, &written, NULL);

			}
			break;

		case scReadFile:
			{
				size_t read;
				FileHandle* fh = (FileHandle*)regs.Rdx;
				fh->read((char**)regs.Rdi, regs.Rcx, &read);
				regs.Rax = (DWORD)read;
				Set_Error(false, regs);
			}
			break;


		case scCloseFile: 
			{
				FileHandle* fh = (FileHandle*)regs.Rdx;
				fh->close();
				Set_Error(false, regs);
				
				//Set_Error(!CloseHandle((HANDLE)regs.Rdx), regs);
			}
			break;

	}
}