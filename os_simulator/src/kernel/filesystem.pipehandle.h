#pragma once
#include "filesystem.fshandle.h" 
#include "filesystem.pipe.h" 


namespace FileSystem {

	class Pipe;
	enum class PIPETYPE;

	class PipeHandle : public FSHandle {
		public:
		
		private:
		PIPETYPE type;
		Pipe* pipe;

		public:		
		PipeHandle(PIPETYPE type, Pipe* pipe);

		/// <summary>
		/// Writes into pipe buffer
		/// </summary>
		/// <param name="buffer">Input from stdout</param>
		/// <param name="buffer_size">Size of input</param>
		/// <param name="pwritten">Number of writted chars</param>
		void write(const void* buffer, const size_t buffer_size, size_t* pwritten);

		fpos_t seek(const fpos_t pos, std::ios_base::seekdir way);

		fpos_t tell();

		/// <summary>
		/// Reads from pipe buffer
		/// </summary>
		/// <param name="buffer">Output to stdin</param>
		/// <param name="buffer_size">Size of buffer to read</param>
		/// <param name="pread">Number of readed chars</param>
		void read(char** buffer, const size_t buffer_size, size_t* pread);

		void close();
	};
		
}
