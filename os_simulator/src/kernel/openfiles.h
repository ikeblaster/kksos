#pragma once
#include <string>
#include <map>
#include <mutex>
#include "..\common\api.h"
#include "filesystem.fshandle.h"



namespace OpenFiles {

	typedef HANDLE OFHandle;

	/// <summary>
	/// Structure which holds pointer to <c>FSHandle</c> and counts references to it.
	/// </summary>
	struct OpenFile
	{
		FileSystem::FSHandle* handle = nullptr;
		size_t refcount = 0;
	};

	/// <summary>
	/// Structure to control exclusivity of objects underlying the handles.
	/// </summary>
	struct OpenFileExclusivity
	{
		bool exclusive_access = false;
		bool shared_read = false;
		size_t ofhcount = 0;
	};


	extern std::map<OFHandle, OpenFile> OpenFiles;

	/// <summary>
	/// Creates new record in table of opened files.
	/// </summary>
	/// <param name="filehandle">Handle to FS object.</param>
	/// <returns>Handle to added record.</returns>
	OFHandle CreateHandle(FileSystem::FSHandle* filehandle);

	/// <summary>
	/// Gets handle to FS object.
	/// </summary>
	/// <param name="handle">Handle to open file record.</param>
	/// <returns>Handle to FS object.</returns>
	FileSystem::FSHandle* GetFSHandle(OFHandle handle);

	/// <summary>
	/// Checks whether specified <c>FSHandle</c> is opened.
	/// </summary>
	/// <param name="filehandle">Handle to FS object.</param>
	/// <returns>Result.</returns>
	bool IsFSHandleOpen(FileSystem::FSHandle* filehandle);

	/// <summary>
	/// Closes opened file record or decreases its reference counter.
	/// </summary>
	/// <param name="handle">Handle to open file record.</param>
	/// <returns>Success</returns>
	bool CloseHandle(OFHandle handle);

}
