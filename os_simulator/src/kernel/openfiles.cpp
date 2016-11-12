#pragma once
#include "openfiles.h"


namespace OpenFiles
{
	std::map<OFHandle, OpenFile> openFiles;

	OFHandle CreateHandle(FileSystem::FSHandle* filehandle) {
		// we use pointer value as key/index; this either find it or creates new record in table
		OpenFile* of = &openFiles[filehandle];

		if (of->handle != nullptr) {
			of->refcount++;
			return filehandle; // TODO: read only rezim
		}

		of->handle = filehandle;
		of->refcount = 1;

		return filehandle;
	}

	FileSystem::FSHandle* GetFSHandle(OFHandle handle) {
		auto search = openFiles.find(handle);
		if (search != openFiles.end()) {
			return search->second.handle;
		}
		return nullptr;
	}

	bool CloseHandle(OFHandle handle) {
		auto search = openFiles.find(handle);
		if (search != openFiles.end()) {
			search->second.refcount--;
			if (search->second.refcount == 0) {
				search->second.handle->close();
				openFiles.erase(handle);
			}
			return true;
		}
		return false;
	}

}
