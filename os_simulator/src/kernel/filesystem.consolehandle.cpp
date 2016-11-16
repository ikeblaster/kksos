#include "filesystem.consolehandle.h"


namespace FileSystem {

	FileSystem::ConsoleHandle* console = nullptr;

	ConsoleHandle::ConsoleHandle()
	{
		_setmode(_fileno(stdin), _O_BINARY);
		mStdIn = GetStdHandle(STD_INPUT_HANDLE);
		//mRedirectedStdIn = (mStdIn != (HANDLE) 3);
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
			// discard everything till newline; because FlushConsoleInputBuffer doesn't do its job
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
			SetConsoleMode(mStdIn, mConsoleMode);
		}
		else if (flags == PROBE__SET_CHARMODE) {
			SetConsoleMode(mStdIn, 0);
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

		if (mStdInOpen && !mStdInClosedByEOF) {
			DWORD lengthtrim = ULONG_MAX;
			if (buffer_size < (size_t) ULONG_MAX) lengthtrim = (DWORD) buffer_size;
			// size_t could be greater than DWORD, so we might have to trim

			BOOL res = ReadFile(mStdIn, buffer, lengthtrim, &read, NULL);

			mStdInOpen = (res) & (read > 0);

			if ((mStdInOpen) & (!mRedirectedStdIn)) {
				mStdInOpen = !
					((read > 2) &&			// there was something before Ctrl+Z
					(buffer[read - 3] == 0x1a) & (buffer[read - 2] == 0x0d) & (buffer[read - 1] == 0x0a));
				if ((!mStdInOpen) & (read > 2)) read -= 3;
				// delete the sequence, if it is necessary
				read = read > 0 ? read : 0;
			}
			else {
				read = mStdInOpen ? read : 0;
			}

			char* eofpos = (char*) memchr((void*) buffer, CHAR_EOF, read);
			if (eofpos != nullptr) {
				mStdInClosedByEOF = true;

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