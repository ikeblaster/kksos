#pragma once
#include <sstream> 
#include "../common/api.h"
#include "filesystem.h"
#include "filesystem.path.h"
#include "filesystem.fshandle.h"


namespace FileSystem {

	class FileHandle : public FSHandle {
		private:
		File* file;
		std::stringstream ss;

		public:
		static FileHandle* CreateHandle(FileSystem::Directory* cwd, char* path, flags_t flags);

		FileHandle(File* file, flags_t flags=0);
		void write(const void* buffer, const size_t buffer_size, size_t* written);
		fpos_t seek(const fpos_t pos, std::ios_base::seekdir way);
		fpos_t tell();
		void read(char** buffer, const size_t buffer_size, size_t* read);
		void close();
		intptr_t getHash();

	};
		
}
