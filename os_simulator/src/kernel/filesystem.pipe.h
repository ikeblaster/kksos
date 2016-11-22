#pragma once
#include <mutex>
#include "filesystem.fshandle.h" 
#include "filesystem.pipehandle.h" 


namespace FileSystem {

	class PipeHandle;
	enum class PIPETYPE { READABLE, WRITEABLE };

	class Pipe {
		private:		
		std::mutex mtx;
		std::condition_variable cv;

		PipeHandle* pipeReadable; // handle to readable end of pipe
		PipeHandle* pipeWriteable; // handle to writeable end of pipe
		bool pipeOpened = true; // flag marking that pipe is still opened

		static const size_t MAX_BUFFER_SIZE = 4096; // maximum buffer size
		char data_buffer[MAX_BUFFER_SIZE]; // buffer for data exchange between pipe ends
		size_t size = 0; // amount of data in buffer
		size_t first = 0; // index - data start
		size_t last = 0; // index - data end

		public:		
		Pipe();
		~Pipe();

		/// <summary>
		/// Destroys this object.
		/// </summary>
		void close();

		/// <summary>
		/// Closes specified end of pipe.
		/// Closes this object when both ends are closed.
		/// </summary>
		/// <param name="type">End of pipe.</param>
		void closePipeHandle(PIPETYPE type);

		/// <summary>
		/// Gets handle to one end of pipe.
		/// </summary>
		/// <param name="type">End of pipe.</param>
		/// <returns>Handle to end of pipe.</returns>
		PipeHandle* getPipeHandle(PIPETYPE type);

		/// <summary>
		/// Writes into pipe buffer.
		/// </summary>
		/// <param name="buffer">Buffer with data to be written into pipe.</param>
		/// <param name="buffer_size">Size of buffer (or input data in buffer).</param>
		/// <param name="pwritten">Number of written bytes.</param>
		void write(const void* buffer, const size_t buffer_size, size_t* pwritten);

		/// <summary>
		/// Reads from pipe buffer.
		/// </summary>
		/// <param name="buffer">Buffer to be filled with data from pipe.</param>
		/// <param name="buffer_size">Size of buffer.</param>
		/// <param name="pread">Number of read bytes.</param>
		void read(char* buffer, const size_t buffer_size, size_t* pread);

	};
		
}
