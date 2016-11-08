#include "vmstdio.h"

thread_local THandle hstdin = nullptr;
thread_local THandle hstdout = nullptr;

std::unique_ptr<const char[]> vmgetline(size_t* pread)
{
	THandle console = Get_Std_Handle(IHANDLE_STDIN);
	auto ret = vmgetline(console, pread);
	return std::move(ret); 
}

std::unique_ptr<const char[]> vmgetline(const THandle file_handle, size_t* read)
{
	std::vector<char> chars;
	char chr[1];
	char* chrp = &chr[0];
	size_t chrread = 0;
	size_t length = 0;

	if (read != nullptr)
		*read = length;

	while (true) {
		Read_File(file_handle, (const void **)&chrp, 1, chrread);
		if (chrread <= 0 && length == 0)
			return nullptr;
		if (chrread <= 0 || chr[0] == '\n' || chr[0] == 26) // 26 = ctrl-z as part of text; see `echo hello ^Z world` in `cmd`
			break;

		chars.push_back(chr[0]);
		length++;
	}

	chars.push_back(0);

	if (read != nullptr)
		*read = length;

	std::unique_ptr<const char[]> ret(_strdup(chars.data()));
	return ret;
}
