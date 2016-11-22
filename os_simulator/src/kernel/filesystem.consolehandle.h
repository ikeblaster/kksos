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
		bool mStdInOpen, mRedirectedStdIn, mStdInClosedByEOF;
		HANDLE mStdIn;
		DWORD mConsoleMode;

		public:
		ConsoleHandle();

		/// <summary>
		/// Gets unique hash representing this handle.
		/// </summary>
		/// <returns>Hash of this handle.</returns>
		intptr_t getExclusivityHash();

		/// <summary>
		/// Gets or sets various things.
		/// Supported flags: 
		/// <c>PROBE__CLEAR_BUFFER</c> clears remaining data in input buffer,
		/// <c>PROBE__IS_INTERACTIVE</c> checks, whether console is interactive or redirected,
		/// <c>PROBE__SET_CHARMODE</c> sets reading to char-by-char mode,
		/// <c>PROBE__SET_LINEMODE</c> sets reading to line mode
		/// </summary>
		/// <returns>Result.</returns>
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
		/// Writes into console (stdout).
		/// </summary>
		/// <param name="buffer">Buffer with data to be written.</param>
		/// <param name="buffer_size">Size of buffer (or input data in buffer).</param>
		/// <param name="pwritten">Number of written bytes.</param>
		void write(const void* buffer, const size_t buffer_size, size_t* pwritten);

		/// <summary>
		/// Reads from console.
		/// Line mode (default) - returns when user hits enter.
		/// Char mode - returns immediately after users hits any key.
		/// </summary>
		/// <param name="buffer">Buffer to be filled with data from pipe.</param>
		/// <param name="buffer_size">Size of buffer.</param>
		/// <param name="pread">Number of read bytes.</param>
		void read(char* buffer, const size_t buffer_size, size_t* pread);

		/// <summary>
		/// Marks console as not closed (!) for further reading.
		/// Ie. process tells this handler that it won't need it anymore.
		/// </summary>
		void close();
	};
		
}
