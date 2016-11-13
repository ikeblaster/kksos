#include "io.h"

using namespace FileSystem;

void HandleIO(CONTEXT &regs) {

	switch (Get_AL((__int16) regs.Rax)) {
		case scCreateFile:
			{
				FSHandle* fh = FileHandle::CreateHandle(Process::current_thread_pcb->current_dir, (char*) regs.Rdx, (flags_t) regs.Rcx);
				if (fh == nullptr) {
					Set_Error(true, regs);
					return;
				}

				THandle fd = Process::add_handle(fh);
				if (fd == nullptr) {
					delete fh; // destroy without notifying = without calling close
					Set_Error(true, regs);
					return;
				}

				regs.Rax = (decltype(regs.Rax)) fd;
			}
			break;


		case scWriteFile:
			{
				FSHandle* fh = Process::get_handle((THandle) regs.Rdx);

				if (fh != nullptr) {
					size_t written = 0;
					fh->write((void*) regs.Rdi, regs.Rcx, &written);
					regs.Rax = (DWORD) written;
				}
				else {
					Set_Error(true, regs);
				}
			}
			break;

		case scReadFile:
			{
				FSHandle* fh = Process::get_handle((THandle) regs.Rdx);

				if (fh != nullptr) {
					size_t read = 0;
					fh->read((char**) regs.Rdi, regs.Rcx, &read);
					regs.Rax = (DWORD) read;
				}
				else {
					Set_Error(true, regs);
				}
			}
			break;

		case scSeekFile:
			{
				FSHandle* fh = Process::get_handle((THandle) regs.Rdx);

				if (fh != nullptr) {
					size_t read = 0;
					regs.Rax = (DWORD) fh->seek((const fpos_t) regs.Rdi, (std::ios_base::seekdir) regs.Rcx);
				}
				else {
					Set_Error(true, regs);
				}
			}
			break;

		case scCloseFile:
			{
				Set_Error(!Process::close_handle((THandle) regs.Rdx), regs);
			}
			break;

		case scCreatePipe:
			{
				Pipe* pipe = new Pipe();

				regs.Rcx = (decltype(regs.Rcx)) Process::add_handle(pipe->getPipeHandle(PIPETYPE::READABLE));
				regs.Rdx = (decltype(regs.Rdx)) Process::add_handle(pipe->getPipeHandle(PIPETYPE::WRITEABLE));
			}
			break;

	}
}