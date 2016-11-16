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
		FileHandle(File* file, flags_t flags=0);
		intptr_t getHash();
		flags_t probe(flags_t flags);
		fpos_t seek(const fpos_t pos, std::ios_base::seekdir way);
		fpos_t tell();
		void write(const void* buffer, const size_t buffer_size, size_t* written);
		void read(char* buffer, const size_t buffer_size, size_t* read);
		void close();

	};
		
}
