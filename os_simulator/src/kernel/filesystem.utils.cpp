#include "filesystem.utils.h"


namespace FileSystem {
	namespace Utils {

		// === FileHandle members

		FileHandle::FileHandle(File* file, bool newFile)
		{
			this->file = file;
			if (this->file->getName() == "CONOUT$") return;
			if(!newFile) this->ss.str(this->file->getData());
			this->seek(0, std::ios_base::beg);
		}

		fpos_t FileHandle::seek(const fpos_t pos, std::ios_base::seekdir way)
		{
			if (this->file->getName() == "CONOUT$") return 0;

			this->ss.clear();
			this->ss.seekg(pos, way);

			this->ss.clear();
			this->ss.seekp(this->ss.tellg()); // sync positions (put->get)

			return this->tell();
		}

		fpos_t FileHandle::tell()
		{
			if (this->file->getName() == "CONOUT$") return 0;

			this->ss.clear();
			return this->ss.tellg();
		}
		
		void FileHandle::write(const void* buffer, const size_t buffer_size, size_t* written)
		{
			if (this->file->getName() == "CONOUT$") {
				printf("%s", (const char*) buffer); // TODO: vyresit nejak lepe, mozna rozhrani/nadrazena trida a jina trida nez FileHandle
			}
			else {
				this->ss.clear();
				this->ss.write((const char*)buffer, buffer_size);

				this->ss.clear();
				this->ss.seekg(this->ss.tellp()); // sync positions (get->put)
			}

			if(written != nullptr)
				*written = buffer_size;
		}

		void FileHandle::read(char** buffer, const size_t buffer_size, size_t* read)
		{
			if (this->file->getName() == "CONOUT$") return;

			this->ss.clear();
			this->ss.read(*buffer, buffer_size);		
			if (read != nullptr)
				*read = this->ss.gcount();
		}

		void FileHandle::close()
		{
			if (this->file->getName() == "CONOUT$") {
				delete this->file;
				return;
			}

			this->ss.clear();
			this->file->setData(this->ss.str());
		}

		// static members

		FileHandle* FileHandle::CreateFileHandle(Directory* cwd, std::string path, size_t flags)
		{
			Directory* directory;
			File* file;

			if (path == "CONOUT$") {
				file = File::createSpecialFile("CONOUT$");
			}
			else {
				RESULT res = Path::parse(cwd, path, &directory, &file);

				if (res == RESULT::MISSING_LAST_PART) {
					file = directory->createFile(Path::getBasename(path));
					res = RESULT::OK;
				}
				if (res != RESULT::OK) return nullptr;
			}

			return new FileHandle(file, (flags & OPEN_EXISTING) == 1);
		}



		// === Path members

		/// <summary>
		/// Parse path in <c>string</c> above filesystem.
		/// </summary>
		/// <param name="cwd">Current working directory</param>
		/// <param name="path">Path</param>
		/// <param name="parsedDirectory"></param>
		/// <param name="parsedFile"></param>
		/// <returns></returns>
		RESULT Path::parse(Directory* cwd, std::string path, Directory** parsedDirectory, File** parsedFile)
		{
			size_t pos = 0, search;
			Directory* directory = cwd;
			File* file = nullptr;

			RESULT retVal = RESULT::OK;
			char* errMsg = nullptr;

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
					errMsg = "file already found, but path continues";
					break;
				}

				// search for semicolon (valid only for first part)
				if (part.find(":") != std::string::npos) {
					if (pos == 0) {
						directory = Path::getDriveRoot(cwd)->getParent();
					}
					else {
						retVal = RESULT::INVALID_PATH;
						errMsg = "invalid char, semicolon somewhere in the middle";
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
						errMsg = "invalid updir";
						retVal = RESULT::INVALID_PATH;
						break; 
					}
					directory = directory->getParent();
				}
				// process path part
				else {
					file = directory->findFile(part); // try to find file
					if (file == nullptr) {
						Directory* tmpdir = directory->findDirectory(part); // or try to find directory

						if (tmpdir == nullptr && search == std::string::npos) {
							errMsg = "Path found except last part, probably file?"; // directory not found, but it's last part of path
							retVal = RESULT::MISSING_LAST_PART;
							break;
						}
						else if (tmpdir == nullptr) {
							errMsg = "Directory not found";
							retVal = RESULT::DIRECTORY_NOT_FOUND;
							break; // file & dir not found
						}
						else {
							directory = tmpdir;
						}
					}
				}

				pos = search + FileSystem::PathSeparator.length();
			} while (search != std::string::npos);

			if (parsedDirectory != nullptr) *parsedDirectory = directory;
			if (parsedFile != nullptr) *parsedFile = file;


			if (errMsg != nullptr) {
				//printf("PATH ERR: %s\n", errMsg); // TODO: predat chybovou hlasku?
			}		
			else {
				Node* node = file;
				if (node == nullptr) node = directory;

				//printf("PARSED: %s\n", generate(node).c_str());
			}

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
				path = current->getName() + (current == node ? "" : "\\" + path);
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
			size_t seplength = FileSystem::PathSeparator.length();
			size_t endpos = path.length();

			while (path.substr(endpos - seplength, seplength) == FileSystem::PathSeparator) {
				endpos -= seplength;
			}
			
			return path.substr(0, endpos);
		}

	}
}