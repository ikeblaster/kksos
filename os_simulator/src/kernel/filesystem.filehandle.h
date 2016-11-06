#pragma once
#include <sstream> 
#include "filesystem.h"
#include "filesystem.path.h"

#define OPEN_EXISTING 3

namespace FileSystem {

	class FileHandle {
		private:
		File* file;
		std::stringstream ss;

		public:
		FileHandle(File* file, bool newFile = true);
		void write(const void* buffer, const size_t buffer_size, size_t* written);
		fpos_t seek(const fpos_t pos, std::ios_base::seekdir way);
		fpos_t tell();
		void read(char** buffer, const size_t buffer_size, size_t* read);
		void close();

		static FileHandle* CreateFileHandle(Directory* cwd, std::string path, size_t flags);
	};
		
}
