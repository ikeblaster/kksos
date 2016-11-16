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
		bool mStdInOpen, mRedirectedStdIn;
		HANDLE mStdIn;
		DWORD mConsoleMode;

		public:
		ConsoleHandle();
		intptr_t getHash();
		flags_t probe(flags_t flags);
		fpos_t seek(const fpos_t pos, std::ios_base::seekdir way);
		fpos_t tell();
		void write(const void* buffer, const size_t buffer_size, size_t* pwritten);
		void read(char* buffer, const size_t buffer_size, size_t* pread);
		void close();
	};
		
}
