#include "vmstdio.h"

std::unique_ptr<const char[]> vmgetline(size_t* pread)
{
	THandle testtxt = Create_File(nullptr, IHANDLE_CONSOLE);
	auto ret = vmgetline(testtxt, pread);
	Close_File(testtxt);
	return std::move(ret); 
}

std::unique_ptr<const char[]> vmgetline(const THandle file_handle, size_t* pread)
{
	std::vector<char> chars;
	char chr[1];
	char* chrp = &chr[0];
	size_t chrread = 0;
	size_t read = 0;

	while (true) {
		Read_File(file_handle, (const void **)&chrp, 1, chrread);
		if (chrread == 0 && read == 0)
			return nullptr;
		if (chrread == 0 || chr[0] == '\n')
			break;

		chars.push_back(chr[0]);
		read++;
	}

	chars.push_back(0);

	if (pread != nullptr)
		*pread = read;

	std::unique_ptr<const char[]> ret(_strdup(chars.data()));
	return ret;
}
