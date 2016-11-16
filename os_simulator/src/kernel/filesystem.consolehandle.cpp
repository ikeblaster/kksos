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
	}

	intptr_t ConsoleHandle::getHash()
	{
		return (intptr_t) this;
	}

	flags_t ConsoleHandle::probe(flags_t flags)
	{
		if (flags == PROBE__CLEAR_BUFFER) {
			// TODO: je tohle ok? Nedela to nekde problemy? Jak funguje 'boot.exe < prikazy'?
			// discard everything till newline; because FlushConsoleInputBuffer doesn't do its job
			if (!mStdInOpen) {
				char buffer[1];
				DWORD written = 1;
				BOOL res = true;
				while (buffer[0] != '\n' && written != 0 && res)
					res = ReadFile(mStdIn, buffer, 1, &written, NULL);
			}

			mStdInOpen = true;
		}
		else if (flags == PROBE__IS_INTERACTIVE) {
			return !mRedirectedStdIn;
		}
		else if (flags == PROBE__SET_BLOCKING) {
			SetConsoleMode(mStdIn, mConsoleMode);
		}
		else if (flags == PROBE__SET_NONBLOCKING) {
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
		int offset = 0;
		DWORD read;

		if (mStdInOpen) {

			DWORD lengthtrim = ULONG_MAX;
			if (buffer_size < (size_t) ULONG_MAX) lengthtrim = (DWORD) buffer_size;
			// size_t could be greater than DWORD, so we might have to trim

			BOOL res = ReadFile(mStdIn, &buffer[offset], lengthtrim, &read, NULL);

			mStdInOpen = (res) & (read > 0);

			//if ((mStdInOpen) & (!mRedirectedStdIn)) {
			//	mStdInOpen = !
			//		((read > 2) &&			// there was something before Ctrl+Z
			//		(buffer[read - 3] == 0x1a) & (buffer[read - 2] == 0x0d) & (buffer[read - 1] == 0x0a));
			//	if ((!mStdInOpen) & (read > 2)) read -= 3;
			//	// delete the sequence, if it is necessary
			//	read = read > 0 ? read : 0;
			//}
			//else {
				read = mStdInOpen ? read : 0;
			//}
		}
		else {
			read = 0; // stdin is no longer open
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