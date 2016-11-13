#pragma once
#include <sstream> 
#include <io.h> 
#include <fcntl.h>
#include <Windows.h> 
#include "filesystem.fshandle.h" 


namespace FileSystem {

	class ConsoleHandle;

	extern FileSystem::ConsoleHandle* console;

	class ConsoleHandle : public FSHandle {
		private:
		bool mStdInOpen;
		HANDLE mStdIn;

		public:
		ConsoleHandle();
		void write(const void* buffer, const size_t buffer_size, size_t* pwritten);
		fpos_t seek(const fpos_t pos, std::ios_base::seekdir way);
		fpos_t tell();
		void read(char** buffer, const size_t buffer_size, size_t* pread);
		void close();
		intptr_t getHash();
	};
		
}
