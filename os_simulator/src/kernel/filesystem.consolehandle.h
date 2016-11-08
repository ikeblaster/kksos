#pragma once
#include <sstream> 
#include <io.h> 
#include <fcntl.h>
#include <Windows.h> 
#include "filesystem.ihandle.h" 


namespace FileSystem {

	class ConsoleHandle : public IHandle {
		private:
		bool mStdInOpen = true;

		public:
		void write(const void* buffer, const size_t buffer_size, size_t* pwritten);
		fpos_t seek(const fpos_t pos, std::ios_base::seekdir way);
		fpos_t tell();
		void read(char** buffer, const size_t buffer_size, size_t* pread);
		void close();
	};
		
}
