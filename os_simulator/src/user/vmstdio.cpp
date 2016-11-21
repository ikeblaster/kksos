#include "vmstdio.h"


std::unique_ptr<const char[]> vmgetline(size_t* pread)
{
	auto ret = vmgetline(THANDLE_STDIN, pread);
	return std::move(ret);
}

std::unique_ptr<const char[]> vmgetline(const THandle file_handle, size_t* read)
{
	std::vector<char> chars;
	unsigned char chr;
	size_t chrread = 0;
	size_t length = 0;

	if (read != nullptr)
		*read = length;

	while (true) {
		if (!Read_File(file_handle, (const void *) &chr, 1, chrread))
			return nullptr;

		if (chrread <= 0 && length == 0)
			return nullptr;
		if (chrread <= 0 || chr == '\n' || chr == CHAR_EOF)
			break;
		if (chr == '\r')
			continue;

		chars.push_back(chr);
		length++;
	}

	chars.push_back(0);

	if (read != nullptr)
		*read = length;

	std::unique_ptr<const char[]> ret(_strdup(chars.data()));
	return ret;
}

const std::vector<std::string>* getArguments(CONTEXT regs)
{
	return (const std::vector<std::string>*) regs.Rcx;
}
