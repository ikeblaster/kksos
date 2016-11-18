#pragma once
#include "openfiles.h"


namespace OpenFiles
{
	// explanation:
	//  - one filehandle can be owned multiple times, hence OpenFile structure with ref. counter
	//  - there can be more filehandles for the same object (file), hence object exclusivity hash counter, 0 = file is not opened anywhere

	std::map<OFHandle, OpenFile> openFiles;
	std::map<intptr_t, OpenFileExclusivity> openFilesExclusivity; 

	OFHandle CreateHandle(FileSystem::FSHandle* filehandle) {
		// we use handle pointer as a key
		OFHandle ofh = (OFHandle) filehandle;
		intptr_t hash = filehandle->getExclusivityHash();
		OpenFileExclusivity* ofe = &openFilesExclusivity[hash];

		// check access privileges only once per handle (ie. when it's not in opened handles already)
		if (openFiles.find(ofh) == openFiles.end()) {

			flags_t fhaccess = filehandle->probe(PROBE__FILE_ACCESS);
			bool wantsExclusive = fhaccess == 0;
			bool wantsSharedRead = ((fhaccess & FH_SHARED_READ) == FH_SHARED_READ);

			if (ofe->exclusive_access || (ofe->ofhcount > 0 && wantsExclusive))
				return nullptr; // file can be in exclussive open only once

			ofe->exclusive_access = wantsExclusive;
			ofe->shared_read = wantsSharedRead;
			ofe->ofhcount++;
		}

		// this will either find it or create a new record in table
		OpenFile* of = &openFiles[ofh];

		if (of->handle == nullptr) {
			of->handle = filehandle;
			of->refcount = 1;
		}
		else {
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
		OFHandle ofh = (OFHandle) filehandle->getExclusivityHash();
		return openFiles.find(ofh) != openFiles.end();
	}

	bool CloseHandle(OFHandle handle) {
		auto search = openFiles.find(handle);
		if (search != openFiles.end()) {

			// openfiles handler
			search->second.refcount--;
			if (search->second.refcount > 0)
				return true;

			// exclusivity counter
			intptr_t hash = search->second.handle->getExclusivityHash();
			OpenFileExclusivity* ofe = &openFilesExclusivity[hash];

			ofe->ofhcount--;

			if (ofe->ofhcount == 0)
				openFilesExclusivity.erase(hash);


			search->second.handle->close();
			openFiles.erase(handle);

			return true;

		}
		return false;
	}

}
