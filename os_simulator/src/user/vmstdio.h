#pragma once
#include <string>
#include <memory>
#include <vector>
#include "rtl.h"


template <typename... Args>
void vmprintf(const char* format, Args... args)
{
	size_t length = std::snprintf(nullptr, 0, format, args...);

	auto buffer = std::make_unique<char[]>(length + 1);
	std::snprintf(buffer.get(), length + 1, format, args...);

	THandle testtxt = Create_File(nullptr, IHANDLE_CONSOLE);
	Write_File(testtxt, buffer.get(), length, length);
	Close_File(testtxt);
}

std::unique_ptr<const char[]> vmgetline(size_t* read = nullptr);
std::unique_ptr<const char[]> vmgetline(const THandle file_handle, size_t* read = nullptr);
