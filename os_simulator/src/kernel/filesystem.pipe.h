#pragma once
#include <mutex>
#include "filesystem.fshandle.h" 
#include "filesystem.pipehandle.h" 


namespace FileSystem {

	class PipeHandle;
	enum class PIPETYPE { READABLE, WRITEABLE };

	class Pipe : public FSHandle {
		private:		
		std::mutex mtx;
		std::condition_variable cv;

		PipeHandle* pipeReadable;
		PipeHandle* pipeWriteable;
		bool pipeOpened = true;

		static const int MAX_BUFFER_SIZE = 5; // max velikost bufferu
		char data_buffer[MAX_BUFFER_SIZE]; // buffer pro vymenu dat ze stdout do stdin
		int size = 0; // pocet nactenych prvku
		int first = 0; // index zacatku bufferu
		int last = 0; // index pro dalsi vkladany prvek

		public:		

		Pipe();

		~Pipe();

		void closePipeHandle(PIPETYPE type);

		PipeHandle * getPipeHandle(PIPETYPE type);

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
