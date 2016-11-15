#pragma once
#include <string>
#include <map>
#include <mutex>
#include "..\common\api.h"
#include "filesystem.fshandle.h"



namespace OpenFiles {

	typedef HANDLE OFHandle;

	typedef struct OpenFile
	{
		FileSystem::FSHandle* handle = nullptr;
		size_t refcount = 0;
	} PCB;


	extern std::map<OFHandle, OpenFile> OpenFiles;

	OFHandle CreateHandle(FileSystem::FSHandle* filehandle);

	FileSystem::FSHandle* GetFSHandle(OFHandle handle);

	bool IsFSHandleOpen(FileSystem::FSHandle * filehandle);

	bool CloseHandle(OFHandle handle);

}
