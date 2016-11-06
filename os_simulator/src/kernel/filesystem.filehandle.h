#pragma once
#include <sstream> 
#include "filesystem.h"
#include "filesystem.path.h"
#include "filesystem.ihandle.h"

#define OPEN_EXISTING 3

namespace FileSystem {

	class FileHandle : public IHandle {
		private:
		File* file;
		std::stringstream ss;

		public:
		FileHandle(File* file, size_t flags=0);
		void write(const void* buffer, const size_t buffer_size, size_t* written);
		fpos_t seek(const fpos_t pos, std::ios_base::seekdir way);
		fpos_t tell();
		void read(char** buffer, const size_t buffer_size, size_t* read);
		void close();
	};
		
}
