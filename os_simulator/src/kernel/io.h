#pragma once
#include "..\common\api.h"
#include "kernel.h"
#include "process.h"
#include "filesystem.filehandle.h"
#include "filesystem.pipe.h"

namespace IO
{
	/// <summary>
	/// Creates new <c>FileHandle</c> to file specified by path.
	/// </summary>
	/// <param name="cwd">Current working directory (starting point for path).</param>
	/// <param name="path">Path to file.</param>
	/// <param name="flags">Flags for filehandle.</param>
	/// <returns>Handle to file.</returns>
	FileSystem::FileHandle* CreateHandle(FileSystem::Directory* cwd, std::string path, flags_t flags);

	/// <summary>
	/// Gets content of directory.
	/// </summary>
	/// <param name="items">[out] Vector of items.</param>
	void list_directory(std::vector<std::string>* items);

	/// <summary>
	/// Checks whether specified directory is opened by some process.
	/// </summary>
	/// <param name="dir">Directory to be checked.</param>
	/// <returns>Result.</returns>
	bool check_directory_open(FileSystem::Directory* dir);

	/// <summary>
	/// Checks whether specified file is opened by some process.
	/// </summary>
	/// <param name="file">File to be checked.</param>
	/// <returns>Result.</returns>
	bool check_file_open(FileSystem::File* file);
}

void HandleIO(CONTEXT &regs);