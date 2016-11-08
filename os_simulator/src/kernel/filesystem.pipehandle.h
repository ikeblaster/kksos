#pragma once
#include <sstream> 
#include <queue>
#include <mutex>
#include "filesystem.ihandle.h" 


namespace FileSystem {

	class PipeHandle : public IHandle {
		private:
		std::queue<char> queue;
		std::mutex mtx;
		std::condition_variable cv;
		bool pipeOpened = true;

		public:
		void write(const void* buffer, const size_t buffer_size, size_t* pwritten);
		fpos_t seek(const fpos_t pos, std::ios_base::seekdir way);
		fpos_t tell();
		void read(char** buffer, const size_t buffer_size, size_t* pread);
		void close();
	};
		
}
