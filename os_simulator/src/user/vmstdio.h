#pragma once
#include <string>
#include <memory>
#include <vector>
#include <thread>
#include "rtl.h"


/// <summary>
/// Prints data to standard output stream.
/// </summary>
/// <param name="format">Formatting.</param>
/// <param name="...args">Format arguments.</param>
template <typename... Args>
void vmprintf(const char* format, Args... args)
{
	vmprintf(THANDLE_STDOUT, format, args...);
}

/// <summary>
/// Prints data to stream.
/// </summary>
/// <param name="handle">Handle to output stream.</param>
/// <param name="format">Formatting.</param>
/// <param name="...args">Format arguments.</param>
template <typename... Args>
void vmprintf(const THandle handle, const char* format, Args... args)
{
	if (handle == nullptr) return;

	size_t length = std::snprintf(nullptr, 0, format, args...);

	auto buffer = std::make_unique<char[]>(length + 1);
	std::snprintf(buffer.get(), length + 1, format, args...);

	Write_File(handle, buffer.get(), length, length);
}

/// <summary>
/// Gets one line from standard input stream.
/// </summary>
/// <param name="read">Number of read characters.</param>
/// <returns>One line.</returns>
std::unique_ptr<const char[]> vmgetline(size_t* read = nullptr);

/// <summary>
/// Gets one line from stream.
/// </summary>
/// <param name="file_handle">Handle to input stream.</param>
/// <param name="read">Number of read characters.</param>
/// <returns>One line.</returns>
std::unique_ptr<const char[]> vmgetline(const THandle file_handle, size_t* read = nullptr);

/// <summary>
/// Gets vector of arguments for process.
/// </summary>
/// <param name="context">Process context.</param>
/// <returns>Vector of arguments.</returns>
const std::vector<std::string>* getArguments(CONTEXT context);
