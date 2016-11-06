#include "filesystem_utils.h"

namespace FileSystem {
	namespace Utils {

		IHandle* CreateHandle(Directory* cwd, std::string path, size_t flags)
		{
			if (path == "CONOUT$") 
				return new ConsoleHandle();
			
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