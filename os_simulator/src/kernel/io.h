#pragma once
#include "..\common\api.h"
#include "kernel.h"
#include "process.h"
#include "filesystem.filehandle.h"
#include "filesystem.pipe.h"

namespace IO
{
	FileSystem::FileHandle* CreateHandle(FileSystem::Directory* cwd, char* path, flags_t flags);
	void list_directory(std::vector<std::string>* items);
	bool check_directory_open(FileSystem::Directory* dir);	
	bool check_file_open(FileSystem::File * file);
}

void HandleIO(CONTEXT &regs);