#pragma once
#include <string>
#include <map>
#include <windows.h> // HRESULT

namespace FileSystem {

	class Directory;
	class File;

	static const std::string PathSeparator = "\\";

	class Node {
		friend class Directory;
		friend class File;

		private:
		Directory* parent;
		std::string name;

		public:
		virtual ~Node();
		Directory* getParent();
		std::string getName();
	};

	class File : public Node {
		private:
		std::string data;

		public:
		~File();
		std::string getData();
		HRESULT setData(std::string data);
	};

	class Directory : public Node {
		friend class File;

		private:
		std::map<std::string, Node*> children;
		Node* getChild(std::string name);

		public:
		~Directory();
		File* createFile(std::string name);
		File* findFile(std::string name);
		HRESULT deleteFile(std::string name);

		Directory* createDirectory(std::string name);
		Directory* findDirectory(std::string name);
		HRESULT deleteDirectory(std::string name);

		HRESULT moveChild(std::string name, Directory* dstdir, std::string dstname);

		std::map<std::string, Node*> getFiles();
	};

}
