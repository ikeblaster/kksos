#pragma once
#include <algorithm>
#include <string>
#include <map>

namespace FileSystem {
	class Directory;
	class File;
}
namespace IO {
	bool check_directory_open(FileSystem::Directory* dir);
	bool check_file_open(FileSystem::File * file);
}

namespace FileSystem {

	extern FileSystem::Directory* fs;
	extern FileSystem::Directory* fs_root;

	enum class RESULT {
		OK,
		GENERAL_ERROR,
		ALREADY_EXISTS,
		NOT_FOUND,
		FILE_ALREADY_EXISTS,
		FILE_NOT_FOUND,
		DIRECTORY_ALREADY_EXISTS,
		DIRECTORY_NOT_FOUND,
		INVALID_PATH,
		MISSING_LAST_PART,
		UNABLE_TO_DELETE
	};

	static const std::string PathSeparator = "\\";

	class Node {
		friend class Directory;
		friend class File;

		private:
		Directory* parent; // parent of file/directory
		std::string name; // name of file/directory

		/// <summary>
		/// Destructor of node. Deletes reference to itself from parent.
		/// </summary>
		virtual ~Node();

		public:
		/// <summary>
		/// Gets parent of file/directory.
		/// </summary>
		/// <returns>Parent directory.</returns>
		Directory* getParent();

		/// <summary>
		/// Gets name of file/directory.
		/// </summary>
		/// <returns>Name.</returns>
		std::string getName();

		/// <summary>
		/// Gets info about file/directory.
		/// </summary>
		/// <returns>Information.</returns>
		virtual std::string toString();

		/// <summary>
		/// Destroys node.
		/// </summary>
		/// <returns>Result.</returns>
		virtual RESULT destroy() = 0;
	};

	class File : public Node {
		private:
		std::string data; // data of file

		public:
		/// <summary>
		/// Gets data of file.
		/// </summary>
		/// <returns>Data of file.</returns>
		std::string getData();

		/// <summary>
		/// Sets new data of file.
		/// </summary>
		/// <param name="data">Data of file.</param>
		void setData(std::string data);

		/// <summary>
		/// Destroys file.
		/// </summary>
		/// <returns>Result.</returns>
		RESULT destroy();
	};

	class Directory : public Node {
		friend class Node;
		friend class File;

		private:
		std::map<std::string, Node*> children; // map of subfiles/subdirectories

		/// <summary>
		/// Finds child by specified name.
		/// </summary>
		/// <param name="name">Name of child.</param>
		/// <returns>Child's object.</returns>
		Node* getChild(std::string name);

		public:
		/// <summary>
		/// Gets name of directory with path separator at the end.
		/// </summary>
		/// <returns>Name.</returns>
		std::string toString();

		/// <summary>
		/// Creates a new file in current directory.
		/// </summary>
		/// <param name="name">Name of file.</param>
		/// <returns>File object.</returns>
		File* createFile(std::string name);
		
		/// <summary>
		/// Finds a file in current directory.
		/// </summary>
		/// <param name="name">Name of file.</param>
		/// <returns>File object.</returns>
		File* findFile(std::string name);

		/// <summary>
		/// Deletes a file in current directory.
		/// </summary>
		/// <param name="name">Name of file.</param>
		/// <returns>Result.</returns>
		RESULT deleteFile(std::string name);


		/// <summary>
		/// Creates a new directory in current directory.
		/// </summary>
		/// <param name="name">Name of directory.</param>
		/// <returns>Directory object.</returns>
		Directory* createDirectory(std::string name);

		/// <summary>
		/// Finds a directory in current directory.
		/// </summary>
		/// <param name="name">Name of directory.</param>
		/// <returns>Directory object.</returns>
		Directory* findDirectory(std::string name);

		/// <summary>
		/// Deletes a directory in current directory.
		/// </summary>
		/// <param name="name">Name of directory.</param>
		/// <returns>Result.</returns>
		RESULT deleteDirectory(std::string name);

		/// <summary>
		/// Move file/directory to new location.
		/// </summary>
		/// <param name="name">Name of file/directory in current directory.</param>
		/// <param name="dstdir">Destination directory.</param>
		/// <param name="dstname">Destination name.</param>
		/// <returns>Result.</returns>
		RESULT moveChild(std::string name, Directory* dstdir, std::string dstname);


		/// <summary>
		/// Gets children of current directory.
		/// </summary>
		/// <returns>Map with children.</returns>
		std::map<std::string, Node*> getFiles();

		/// <summary>
		/// Destroys directory.
		/// </summary>
		/// <returns>Result.</returns>
		RESULT destroy();
	};

}
