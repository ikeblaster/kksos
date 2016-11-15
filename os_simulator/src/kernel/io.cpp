#include "io.h"

using namespace FileSystem;

namespace IO
{
	FileHandle* CreateHandle(Directory* cwd, char* path, flags_t flags)
	{
		Directory* directory;
		File* file = nullptr;

		RESULT res = Path::parse(cwd, path, &directory, &file, 0);

		if (res == RESULT::MISSING_LAST_PART) {

			bool create_missing = ((flags & FH_OPEN_OR_CREATE) == FH_OPEN_OR_CREATE)
				|| ((flags & FH_CREATE_ALWAYS) == FH_CREATE_ALWAYS);

			if (!create_missing) {
				return nullptr;
			}

			file = directory->createFile(Path::getBasename(path));
		}

		if (file == nullptr)
			return nullptr;

		return new FileHandle(file, flags);
	}

	void list_directory(std::vector<std::string>* items) 
	{
		auto files = Process::current_thread_pcb->current_dir->getFiles();
		for (auto file : files) {
			items->push_back(file.second->toString());
		}
	}

	bool check_directory_open(FileSystem::Directory* dir)
	{
		for (int i = 0; i < Process::PROCESS_TABLE_SIZE; i++) {
			if (Process::table[i] != nullptr && Process::table[i]->current_dir == dir)
				return true;
		}

		return false;
	}

	bool check_file_open(FileSystem::File* file)
	{
		FileHandle fh(file, 0);
		bool ret = OpenFiles::IsFSHandleOpen(&fh);
		return ret;
	}
}


void HandleIO(CONTEXT &regs) {

	switch (Get_AL((__int16) regs.Rax)) {
		case scCreateFile:
			{
				FileHandle* fh = IO::CreateHandle(Process::current_thread_pcb->current_dir, (char*) regs.Rdx, (flags_t) regs.Rcx);
				if (fh == nullptr) {
					Set_Error(true, regs);
					break;
				}

				THandle fd = Process::add_handle(fh);
				if (fd == nullptr) {
					delete fh; // destroy without notifying = without calling close
					Set_Error(true, regs);
					break;
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
					regs.Rax = (decltype(regs.Rax)) written;
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
					regs.Rax = (decltype(regs.Rax)) read;
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
					regs.Rax = (decltype(regs.Rax)) fh->seek((const fpos_t) regs.Rdi, (std::ios_base::seekdir) regs.Rcx);
				}
				else {
					Set_Error(true, regs);
				}
			}
			break;

		case scCloseFile:
			{
				bool res = Process::close_handle((THandle) regs.Rdx);
				Set_Error(!res, regs);
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
				RESULT res = Path::parse(Process::current_thread_pcb->current_dir, *(std::string*) regs.Rdx, nullptr, nullptr, FS_MAKE_MISSING_DIRS);
				Set_Error(res != RESULT::OK, regs);
			}
			break;

		case scRemoveDirectory:
			{
				Directory* directory = nullptr;
				Path::parse(Process::current_thread_pcb->current_dir, *(std::string*) regs.Rdx, &directory, nullptr, 0);

				if (directory == nullptr || directory->destroy() != RESULT::OK) {
					Set_Error(true, regs);
				}
			}
			break;

		case scListDirectory:
			{
				IO::list_directory((std::vector<std::string>*) regs.Rdx);
			}
			break;

		case scProbeFile:
			{
				FSHandle* fh = Process::get_handle((THandle) regs.Rdx);

				if (fh != nullptr) {
					regs.Rax = (decltype(regs.Rax)) fh->probe((const flags_t) regs.Rcx);
				}
				else {
					Set_Error(true, regs);
				}
			}
			break;
	}

}