#pragma once
#include <sstream> 
#include "filesystem.h"
#include "filesystem.ihandle.h"
#include "filesystem.consolehandle.h"
#include "filesystem.filehandle.h"

#define OPEN_EXISTING 3

namespace FileSystem {
	namespace Utils {

		IHandle* CreateHandle(FileSystem::Directory* cwd, std::string path, size_t flags);

	}
}
