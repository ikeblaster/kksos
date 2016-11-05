#include "filesystem.utils.h"


namespace FileSystem {
	namespace Utils {

		// === FileHandle members
		FileHandle::FileHandle(File* file, bool newFile)
		{
			this->file = file;
			if(!newFile) this->ss.str(this->file->getData());
			this->seek(0, std::ios_base::beg);
		}

		fpos_t FileHandle::seek(const fpos_t pos, std::ios_base::seekdir way)
		{
			this->ss.clear();
			this->ss.seekg(pos, way);

			this->ss.clear();
			this->ss.seekp(this->ss.tellg()); // sync positions (put->get)

			return this->tell();
		}

		fpos_t FileHandle::tell()
		{
			this->ss.clear();
			return this->ss.tellg();
		}
		
		void FileHandle::write(const void* buffer, const size_t buffer_size, size_t* written)
		{
			this->ss.clear();
			this->ss.write((const char*) buffer, buffer_size);

			this->ss.clear();
			this->ss.seekg(this->ss.tellp()); // sync positions (get->put)

			if(written != nullptr)
				*written = buffer_size;
		}

		void FileHandle::read(char** buffer, const size_t buffer_size, size_t* read)
		{
			this->ss.clear();
			this->ss.read(*buffer, buffer_size);		
			if (read != nullptr)
				*read = this->ss.gcount();
		}

		void FileHandle::close()
		{
			this->ss.clear();
			this->file->setData(this->ss.str());
		}




		// === Path members

		/// <summary>
		/// Parse path in <c>string</c> above filesystem.
		/// </summary>
		/// <param name="cwd">Current working directory</param>
		/// <param name="path">Path</param>
		/// <returns></returns>
		HRESULT Path::parse(Directory* cwd, std::string path)
		{
			size_t pos = 0, search;
			Directory* directory = cwd;
			File* file = nullptr;

			do
			{
				search = path.find(FileSystem::PathSeparator, pos);
				std::string part = path.substr(pos, search - pos);
				
				if (file != nullptr) {
					file = nullptr;
					directory = nullptr;

					printf("file already found, but path continues\n");
					return S_FALSE; // file already found, but path continues
				}

				if (part.find(":") != std::string::npos) {
					if (pos == 0) {
						directory = Path::getDriveRoot(cwd)->getParent();
					}
					else {
						printf("invalid char, semicolon somewhere in the middle\n");
						return S_FALSE; // invalid char, semicolon somewhere in the middle
					}
				}

				if (pos == 0 && search == 0) {
					directory = Path::getDriveRoot(cwd); // if path begins with "\"
				}
				else if (part == "..") {
					if (directory->getParent() == nullptr || directory->getParent()->getParent() == nullptr) {
						printf("invalid updir\n");
						return S_FALSE; // invalid updir
					}
					directory = directory->getParent();
				}
				else if (part.empty() || part == ".") { 
				}
				else {
					file = directory->findFile(part); // try to find file
					if (file == nullptr) {
						directory = directory->findDirectory(part); // or try to find directory
						if (directory == nullptr) {
							printf("%s not found\n", part.c_str());
							return S_FALSE; // file&dir not found
						}
					}
				}

				pos = search + FileSystem::PathSeparator.length();
			} while (search != std::string::npos);

			Node* node = file;
			if (node == nullptr) node = directory;

			printf("PARSED: %s\n", generate(node).c_str());

			return 1;
		}

		/// <summary>
		/// Get drive root for directory.
		/// For <c>File</c> use <c>file->getParent()</c>.
		/// </summary>
		/// <param name="directory">Any <c>Directory</c></param>
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
		/// <param name="node"><c>File</c> or <c>Directory</c></param>
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

	}
}