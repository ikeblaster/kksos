#include "filesystem.path.h"


namespace FileSystem {

	/// <summary>
	/// Parse path in <c>string</c> above filesystem.
	/// </summary>
	/// <param name="cwd">Current working directory</param>
	/// <param name="path">Path</param>
	/// <param name="parsedDirectory"></param>
	/// <param name="parsedFile"></param>
	/// <returns></returns>
	RESULT Path::parse(Directory* cwd, std::string path, Directory** parsedDirectory, File** parsedFile, flags_t flags)
	{
		size_t pos = 0, search;
		Directory* directory = cwd;
		File* file = nullptr;
		RESULT retVal = RESULT::OK;

		path = Path::trimTrailingSeparators(path);

		do
		{
			search = path.find(FileSystem::PathSeparator, pos);
			std::string part = path.substr(pos, search - pos);
				
			// file already found, but path continues
			if (file != nullptr) {
				file = nullptr;
				directory = nullptr;

				retVal = RESULT::INVALID_PATH;
				break;
			}

			// search for semicolon
			if (part.find(":") != std::string::npos) {
				if (pos == 0 && (part.find(":") == part.length() - 1)) { 
					directory = Path::getDriveRoot(cwd)->getParent(); // semicolon is valid only for first part and must be last char
				}
				else {
					retVal = RESULT::INVALID_PATH; 
					break;
				}
			}

			// path relative to root
			if (pos == 0 && search == 0) {
				directory = Path::getDriveRoot(cwd); // if path begins with "\"
			}
			// empty part
			else if (part.empty() || part == ".") {
			}
			// updir
			else if (part == "..") {
				if (directory->getParent() == nullptr || directory->getParent()->getParent() == nullptr) { // forbid FS root (parent of C:)
					retVal = RESULT::INVALID_PATH; // invalid updir
					break; 
				}
				directory = directory->getParent();
			}
			// process path part
			else {
				file = directory->findFile(part); // try to find file
				if (file == nullptr) {
					Directory* tmpdir = directory->findDirectory(part); // or try to find directory

					if(tmpdir != nullptr) {
						directory = tmpdir; // directory found
					}
					else if ((flags & FS_MAKE_MISSING_DIRS) == FS_MAKE_MISSING_DIRS) {
						directory = directory->createDirectory(part); // directory in path not found then make one
					}
					else if (search == std::string::npos) {
						retVal = RESULT::MISSING_LAST_PART; // directory not found, but it's last part of path (probably file)
						break;
					}
					else {
						retVal = RESULT::DIRECTORY_NOT_FOUND; // file & dir not found
						break; 
					}
				}
			}

			pos = search + FileSystem::PathSeparator.length();
		} while (search != std::string::npos);

		if (parsedDirectory != nullptr) *parsedDirectory = directory;
		if (parsedFile != nullptr) *parsedFile = file;


		return retVal;
	}

	/// <summary>
	/// Returns trailing name component of path
	/// </summary>
	/// <param name="path">A path.</param>
	/// <returns>Returns the base name of the given <c>path</c>.</returns>
	std::string Path::getBasename(std::string path)
	{
		path = Path::trimTrailingSeparators(path);

		size_t pos = path.rfind(FileSystem::PathSeparator);
		if (pos != std::string::npos)
			return path.substr(pos + FileSystem::PathSeparator.length());

		return path;
	}

	/// <summary>
	/// Get drive root for directory.
	/// For <c>File</c> use <c>file->getParent()</c>.
	/// </summary>
	/// <param name="directory">Any <c>Directory</c>.</param>
	/// <returns><c>Directory</c> representing drive root.</returns>
	Directory* Path::getDriveRoot(Directory* directory)
	{
		Directory* root = directory;
		if (root != nullptr && root->getParent() != nullptr) {
			while (root->getParent()->getParent() != nullptr) {
				root = root->getParent(); // drive has parent (FS root), but no grandparent
			}
		}
		return root;
	}

	/// <summary>
	/// Generate <c>string</c> representing path.
	/// </summary>
	/// <param name="node"><c>File</c> or <c>Directory</c>.</param>
	std::string Path::generate(Node* node)
	{
		std::string path;
		Node* current = node;

		while (current != nullptr && current->getParent() != nullptr) {
			path = current->getName() + (current == node ? "" : FileSystem::PathSeparator + path);
			current = current->getParent();
		}

		return path;
	}

	/// <summary>
	/// Trims all trailing path separators.
	/// </summary>
	/// <param name="path">A path.</param>
	/// <returns><c>path</c> without trailing separators.</returns>
	std::string Path::trimTrailingSeparators(std::string path)
	{
		if (path.length() == 0) 
			return path;

		size_t seplength = FileSystem::PathSeparator.length();
		size_t endpos = path.length();

		while (endpos > seplength && path.substr(endpos - seplength, seplength) == FileSystem::PathSeparator) {
			endpos -= seplength;
		}
			
		return path.substr(0, endpos);
	}

}