#pragma once
#include "openfiles.h"


namespace OpenFiles
{
	std::map<OFHandle, OpenFile> openFiles;

	OFHandle CreateHandle(FileSystem::FSHandle* filehandle) {
		// we use pointer value (inptr_t) as key/index
		OFHandle ofh = (OFHandle) filehandle->getHash();

		// this either find it or create a new record in table
		OpenFile* of = &openFiles[ofh];

		if (of->handle == nullptr) {
			of->handle = filehandle;
			of->refcount = 1;
		}
		else {
			if(of->handle != filehandle)
				delete filehandle; // use filehandle saved in openFiles instead

			of->refcount++;
		}

		return ofh;
	}

	FileSystem::FSHandle* GetFSHandle(OFHandle ofh) {
		auto search = openFiles.find(ofh);
		if (search != openFiles.end()) {
			return search->second.handle;
		}
		return nullptr;
	}

	bool IsFSHandleOpen(FileSystem::FSHandle* filehandle) {
		OFHandle ofh = (OFHandle) filehandle->getHash();
		return openFiles.find(ofh) != openFiles.end();
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
