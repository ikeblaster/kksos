#include "filesystem.h"

namespace FS {

	// === Node members

	Node::~Node()
	{
	}

	Directory* Node::getParent()
	{
		return this->parent;
	}


	// === File members

	std::string File::getData()
	{
		return this->data;
	}


	// === Directory members

	Node* Directory::getChild(std::string name)
	{
		auto search = this->children.find(name);
		if (search != this->children.end()) {
			return search->second;
		}
		return nullptr;
	}

	File* Directory::createFile(std::string name)
	{
		if (this->getChild(name) != nullptr)
			return nullptr; // already exists

		File* file = new File();
		file->parent = this;
		this->children.insert(std::make_pair(name, file));
		return file;
	}

	File* Directory::getFile(std::string name)
	{
		Node* node = this->getChild(name);
		File* file = dynamic_cast<File*>(node);
		return file == NULL ? nullptr : file;
	}

	Directory* Directory::createDir(std::string name)
	{
		if (this->getChild(name) != nullptr)
			return nullptr; // already exists

		Directory* dir = new Directory();
		dir->parent = this;
		this->children.insert(std::make_pair(name, dir));
		return dir;
	}

	Directory* Directory::getDir(std::string name)
	{
		Node* node = this->getChild(name);
		Directory* dir = dynamic_cast<Directory*>(node);
		return dir == NULL ? nullptr : dir;
	}

	std::map<std::string, Node*> Directory::getFiles()
	{
		return this->children;
	}

	HRESULT Directory::deleteFile(std::string name)
	{
		File* file = this->getFile(name);
		if (file == nullptr)
			return S_FALSE;

		delete file;
		this->children.erase(name);

		return S_OK;
	}

	HRESULT Directory::deleteDir(std::string name)
	{
		Directory* dir = this->getDir(name);
		if (dir == nullptr)
			return S_FALSE;

		for (auto item : dir->children) {
			Directory* subdir = dynamic_cast<Directory*>(item.second);
			if (subdir != nullptr)
				this->deleteDir(item.first);
			else
				this->deleteFile(item.first);
		}

		delete dir;
		this->children.erase(name);

		return S_OK;
	}

	HRESULT Directory::moveChild(std::string name, Directory* dstdir, std::string dstname)
	{
		if (name == dstname && dstdir == this)
			return S_OK; // src = dst

		if (dstdir->getChild(dstname) != nullptr)
			return S_FALSE; // dstname already exists in dst directory

		Node* node = this->getChild(name); // find src node
		if (node == nullptr)
			return S_FALSE; // node not found

		this->children.erase(name); // delete from src directory

		node->parent = dstdir; // set new parent
		dstdir->children.insert(std::make_pair(dstname, node)); // insert into dst directory

		return S_OK;
	}


}