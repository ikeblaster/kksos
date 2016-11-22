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
		bool sharedRead;

		public:
		FileHandle(File* file, flags_t flags=0);

		/// <summary>
		/// Gets unique hash representing this handle.
		/// </summary>
		/// <returns>Hash of this handle.</returns>
		intptr_t getExclusivityHash();

		/// <summary>
		/// Gets or sets various things.
		/// Supported flags: 
		/// <c>PROBE__FILE_ACCESS</c> checks file access mode (shared/exclussive).
		/// </summary>
		/// <returns>Result.</returns>
		flags_t probe(flags_t flags);

		/// <summary>
		/// Changes cursor position in file.
		/// </summary>
		/// <param name="pos">New position.</param>
		/// <param name="way">Direction for setting new position.</param>
		/// <returns>New position in file.</returns>
		fpos_t seek(const fpos_t pos, std::ios_base::seekdir way);

		/// <summary>
		/// Gets cursor position in file.
		/// </summary>
		/// <returns>Position in file.</returns>
		fpos_t tell();

		/// <summary>
		/// Writes into file.
		/// </summary>
		/// <param name="buffer">Buffer with data to be written.</param>
		/// <param name="buffer_size">Size of buffer (or input data in buffer).</param>
		/// <param name="pwritten">Number of written bytes.</param>
		void write(const void* buffer, const size_t buffer_size, size_t* written);

		/// <summary>
		/// Reads from file.
		/// </summary>
		/// <param name="buffer">Buffer to be filled with data from pipe.</param>
		/// <param name="buffer_size">Size of buffer.</param>
		/// <param name="pread">Number of read bytes.</param>
		void read(char* buffer, const size_t buffer_size, size_t* read);

		/// <summary>
		/// Closes file and pushes new content of file into file system.
		/// </summary>
		void close();

	};
		
}
