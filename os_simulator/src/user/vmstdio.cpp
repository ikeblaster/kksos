#include "vmstdio.h"


std::unique_ptr<const char[]> vmgetline(size_t* pread)
{
	auto ret = vmgetline(THANDLE_STDIN, pread);
	return std::move(ret);
}

std::unique_ptr<const char[]> vmgetline(const THandle file_handle, size_t* read)
{
	std::vector<char> chars;
	char chr;
	size_t chrread = 0;
	size_t length = 0;

	if (read != nullptr)
		*read = length;

	while (true) {
		if (!Read_File(file_handle, (const void *) &chr, 1, chrread))
			return nullptr;

		if (chrread <= 0 && length == 0)
			return nullptr;
		if (chrread <= 0 || chr == '\n' || chr == 26) // 26 = ctrl-z as part of text; see `echo hello ^Z world` in `cmd`
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
