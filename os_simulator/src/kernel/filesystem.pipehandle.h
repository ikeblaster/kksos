#pragma once
#include "filesystem.fshandle.h" 
#include "filesystem.pipe.h" 


namespace FileSystem {

	class Pipe;
	enum class PIPETYPE;

	class PipeHandle : public FSHandle {
		private:
		PIPETYPE type;
		Pipe* pipe;

		public:		
		PipeHandle(PIPETYPE type, Pipe* pipe);

		/// <summary>
		/// Gets unique hash representing this handle.
		/// </summary>
		/// <returns>Hash of this handle.</returns>
		intptr_t getExclusivityHash();

		/// <summary>
		/// Does nothing.
		/// </summary>
		/// <returns>0</returns>
		flags_t probe(flags_t flags);

		/// <summary>
		/// Does nothing.
		/// </summary>
		/// <returns>0</returns>
		fpos_t seek(const fpos_t pos, std::ios_base::seekdir way);

		/// <summary>
		/// Does nothing.
		/// </summary>
		/// <returns>0</returns>
		fpos_t tell();

		/// <summary>
		/// Writes into pipe buffer. Suspends caller when buffer is full.
		/// </summary>
		/// <param name="buffer">Buffer with data to be written into pipe.</param>
		/// <param name="buffer_size">Size of buffer (or input data in buffer).</param>
		/// <param name="pwritten">Number of written bytes.</param>
		void write(const void* buffer, const size_t buffer_size, size_t* pwritten);

		/// <summary>
		/// Reads from pipe buffer. Suspends caller when buffer is empty.
		/// </summary>
		/// <param name="buffer">Buffer to be filled with data from pipe.</param>
		/// <param name="buffer_size">Size of buffer.</param>
		/// <param name="pread">Number of read bytes.</param>
		void read(char* buffer, const size_t buffer_size, size_t* pread);

		/// <summary>
		/// Closes handle to pipe.
		/// </summary>
		void close();

	};
		
}
