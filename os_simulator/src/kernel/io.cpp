#include "io.h"

using namespace FileSystem;

namespace IO
{
	void list_directory(std::vector<std::string>* items) 
	{
		auto files = Process::current_thread_pcb->current_dir->getFiles();
		for (auto file : files) {
			items->push_back(file.first);
		}
	}
}


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


		case scMakeDirectory:
			{
				Directory* directory;
				File* file;

				RESULT res = Path::parse(Process::current_thread_pcb->current_dir, *(std::string*) regs.Rdx, &directory, &file, FS_MAKE_MISSING_DIRS);

				regs.Rax = (decltype(regs.Rax)) (res == RESULT::OK);
			}
			break;



		case scListDirectory:
			{
				IO::list_directory((std::vector<std::string>*) regs.Rdx);
			}
			break;



		case scRemoveDirectory:
			{
				Directory* directory = nullptr;
				File* file;

				Path::parse(Process::current_thread_pcb->current_dir, *(std::string*) regs.Rdx, &directory, &file, 0);
				delete directory;

			}
			break;

	}

}