#include "filesystem.consolehandle.h"


namespace FileSystem {

	FileSystem::ConsoleHandle* console = nullptr;

	ConsoleHandle::ConsoleHandle()
	{
		_setmode(_fileno(stdin), _O_BINARY);
		mStdIn = GetStdHandle(STD_INPUT_HANDLE);
		mRedirectedStdIn = !GetConsoleMode(mStdIn, &mConsoleMode);

		mStdInOpen = true;
		mStdInClosedByEOF = false;
	}

	intptr_t ConsoleHandle::getHash()
	{
		return (intptr_t) this;
	}

	flags_t ConsoleHandle::probe(flags_t flags)
	{
		if (flags == PROBE__CLEAR_BUFFER) {

			// discard everything till newline
			if (mStdInClosedByEOF) {
				char buffer;
				DWORD written;

				while (ReadFile(mStdIn, &buffer, 1, &written, NULL) && buffer != '\n' && written != 0);
			}

			mStdInOpen = true;
			mStdInClosedByEOF = false;
		}
		else if (flags == PROBE__IS_INTERACTIVE) {
			return !mRedirectedStdIn;
		}
		else if (flags == PROBE__SET_LINEMODE) {
			SetConsoleMode(mStdIn, mConsoleMode | ENABLE_LINE_INPUT);
			mStdInClosedByEOF = false;
		}
		else if (flags == PROBE__SET_CHARMODE) {
			SetConsoleMode(mStdIn, mConsoleMode & ~ENABLE_LINE_INPUT);
		}

		return 0;
	}

	fpos_t ConsoleHandle::seek(const fpos_t pos, std::ios_base::seekdir way)
	{
		return 0;
	}

	fpos_t ConsoleHandle::tell()
	{
		return 0;
	}

	void ConsoleHandle::write(const void* buffer, const size_t buffer_size, size_t* pwritten)
	{
		DWORD written;
		WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), buffer, (DWORD) buffer_size, &written, NULL);

		if (pwritten != nullptr)
			*pwritten = written;
	}

	void ConsoleHandle::read(char* buffer, const size_t buffer_size, size_t* pread)
	{
		DWORD read = 0;

		if (mStdInOpen) {
			DWORD sizetrim = (buffer_size > (size_t) ULONG_MAX) ? ULONG_MAX : (DWORD) buffer_size; // size_t could be greater than DWORD, so we might have to trim
			BOOL res;


			// ReadConsoleA works better when input buffer contains ^Z, but it doesn't work for redirected stdin.
			// We force reading char by char for redirected stdin to catch ^Z in the middle of data.
			// Otherwise everything after ^Z is also in buffer => it's lost.
			if (mRedirectedStdIn)
				res = ReadFile(mStdIn, buffer, 1, &read, NULL);
			else
				res = ReadConsoleA(mStdIn, buffer, sizetrim, &read, NULL);

			mStdInOpen = (res) & (read > 0);
			read = mStdInOpen ? read : 0;

			char* eofpos = (char*) memchr((void*) buffer, CHAR_EOF, read);
			if (eofpos != nullptr) {
				mStdInOpen = false;

				mStdInClosedByEOF = (buffer[read - 1] != '\n'); // If last character wasn't newline, then there is something in the buffer waiting => clear it on demand

				*eofpos = 0;
				read = (DWORD) (eofpos - buffer);
				if (read < 0) read = 0;
			}
		}

		if (pread != nullptr)
			*pread = read;
	}

	void ConsoleHandle::close()
	{
		mStdInOpen = true;
		// don't commit suicide
	}

}