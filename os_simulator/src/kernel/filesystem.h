#pragma once
#include <string>
#include <map>

namespace FileSystem {
	class Directory;
	class File;
}
namespace Process {
	bool check_cwd(FileSystem::Directory* dir);
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
		virtual ~Node();
		Directory* parent;
		std::string name;

		public:
		Directory* getParent();
		std::string getName();
		virtual RESULT destroy() = 0;
	};

	class File : public Node {
		private:
		std::string data;

		public:
		std::string getData();
		void setData(std::string data);
		RESULT destroy();
	};

	class Directory : public Node {
		friend class Node;
		friend class File;

		private:
		std::map<std::string, Node*> children;
		Node* getChild(std::string name);

		public:
		File* createFile(std::string name);
		File* findFile(std::string name);
		RESULT deleteFile(std::string name);

		Directory* createDirectory(std::string name);
		Directory* findDirectory(std::string name);
		RESULT deleteDirectory(std::string name);

		RESULT moveChild(std::string name, Directory* dstdir, std::string dstname);

		std::map<std::string, Node*> getFiles();

		RESULT destroy();
	};

}
