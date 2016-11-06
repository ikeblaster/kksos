#pragma once
#include <sstream> 
#include "../common/api.h"
#include "filesystem.h"
#include "filesystem.ihandle.h"
#include "filesystem.consolehandle.h"
#include "filesystem.filehandle.h"


namespace FileSystem {
	namespace Utils {

		IHandle* CreateHandle(FileSystem::Directory* cwd, char* path, size_t flags);

	}
}
