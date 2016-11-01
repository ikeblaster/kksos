#pragma once

#include <string>
#include <sstream> 
#include <map>
#include <windows.h>

namespace FS {

	class Directory;
	class File;

	class Node {
		friend class Directory;
		friend class File;

		private:
		Directory* parent;

		public:
		virtual ~Node();
		Directory* getParent();
	};

	class File : public Node {
		private:
		std::string data;

		public:
		std::string getData();
		HRESULT setData(std::string data);
	};

	class Directory : public Node {
		private:
		std::map<std::string, Node*> children;
		Node* getChild(std::string name);

		public:
		~Directory();
		File* createFile(std::string name);
		File* getFile(std::string name);
		HRESULT deleteFile(std::string name);

		Directory* createDir(std::string name);
		Directory* getDir(std::string name);
		HRESULT deleteDir(std::string name);

		HRESULT moveChild(std::string name, Directory* dstdir, std::string dstname);

		std::map<std::string, Node*> getFiles();
	};

	class FileHandle {
		private:
		File* file;
		std::stringstream ss;

		public:
		FileHandle(File* file);
		void writeFile(const void *buffer, const size_t buffer_size, size_t &written);
		void closeFile();
	};

}
