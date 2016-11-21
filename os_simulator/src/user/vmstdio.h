#pragma once
#include <string>
#include <memory>
#include <vector>
#include <thread>
#include "rtl.h"


template <typename... Args>
void vmprintf(const char* format, Args... args)
{
	vmprintf(THANDLE_STDOUT, format, args...);
}

template <typename... Args>
void vmprintf(const THandle handle, const char* format, Args... args)
{
	if (handle == nullptr) return;

	size_t length = std::snprintf(nullptr, 0, format, args...);

	auto buffer = std::make_unique<char[]>(length + 1);
	std::snprintf(buffer.get(), length + 1, format, args...);

	Write_File(handle, buffer.get(), length, length);
}

std::unique_ptr<const char[]> vmgetline(size_t* read = nullptr);
std::unique_ptr<const char[]> vmgetline(const THandle file_handle, size_t* read = nullptr);

const std::vector<std::string>* getArguments(CONTEXT context);
