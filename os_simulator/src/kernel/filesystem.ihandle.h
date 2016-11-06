#pragma once
#include <sstream> 
#include "filesystem.h"

#define OPEN_EXISTING 3

namespace FileSystem {

	class IHandle {
		public:

		virtual ~IHandle() {};
		virtual void write(const void* buffer, const size_t buffer_size, size_t* written) = 0;
		virtual fpos_t seek(const fpos_t pos, std::ios_base::seekdir way) = 0;
		virtual fpos_t tell() = 0;
		virtual void read(char** buffer, const size_t buffer_size, size_t* read) = 0;
		virtual void close() = 0;
	};
		
}
