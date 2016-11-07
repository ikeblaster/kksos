#include "filesystem_utils.h"


namespace FileSystem {
	namespace Utils {

		IHandle* CreateHandle(Directory* cwd, char* path, size_t flags)
		{
			if ((flags & IHANDLE_CONSOLE) == IHANDLE_CONSOLE)
				return new ConsoleHandle();

			if ((flags & IHANDLE_PIPE) == IHANDLE_PIPE)
				return new PipeHandle();
			
			Directory* directory;
			File* file = nullptr;

			RESULT res = Path::parse(cwd, path, &directory, &file);

			if (res == RESULT::MISSING_LAST_PART) 
				file = directory->createFile(Path::getBasename(path));
			
			if (file == nullptr)
				return nullptr;

			return new FileHandle(file, flags);
		}


	}
}