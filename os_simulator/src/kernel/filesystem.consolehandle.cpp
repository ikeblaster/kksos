#include "filesystem.consolehandle.h"

namespace FileSystem {

	fpos_t ConsoleHandle::seek(const fpos_t pos, std::ios_base::seekdir way)
	{
		return 0;
	}

	fpos_t ConsoleHandle::tell()
	{
		return 0;
	}
		
	void ConsoleHandle::write(const void* buffer, const size_t buffer_size, size_t* written)
	{
		printf((const char*) buffer); // TODO: resit nejak jinak nez printf?

		if (written != nullptr)
			*written = buffer_size;
	}

	void ConsoleHandle::read(char** buffer, const size_t buffer_size, size_t* read)
	{
		if (read != nullptr)
			*read = 0;
	}

	void ConsoleHandle::close()
	{

	}

}