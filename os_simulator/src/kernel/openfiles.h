#pragma once
#include <string>
#include <map>
#include <mutex>
#include "..\common\api.h"
#include "filesystem.fshandle.h"



namespace OpenFiles {

	typedef HANDLE OFHandle;

	const size_t OPEN_FILES_TABLE_SIZE = 32768;

	typedef struct OpenFile
	{
		FileSystem::FSHandle* handle = nullptr;
		size_t refcount = 0;
	} PCB;


	extern std::map<OFHandle, OpenFile> OpenFiles;

	OFHandle CreateHandle(FileSystem::FSHandle* filehandle);

	FileSystem::FSHandle* GetFSHandle(OFHandle handle);

	bool CloseHandle(OFHandle handle);

}
