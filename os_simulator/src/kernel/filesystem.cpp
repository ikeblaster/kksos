#include "filesystem.h"

namespace FileSystem {

	// === Node members

	Node::~Node()
	{
	}

	Directory* Node::getParent()
	{
		return this->parent;
	}

	std::string Node::getName()
	{
		return this->name;
	}


	// === File members

	File::~File()
	{
		this->parent->children.erase(this->name);
	}

	std::string File::getData()
	{
		return this->data;
	}

	HRESULT File::setData(std::string data)
	{
		this->data = data;
		return S_OK;
	}


	// === Directory members

	Directory::~Directory() {
		while(!this->children.empty()) {
			auto item = *(this->children.begin());
			delete item.second; // calls Directory or File dtor
		}

		if (this->parent != nullptr) 
			this->parent->children.erase(this->name); // delete from parents children
	}

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
		if (name.empty()) 
			return nullptr; // empty name

		if (this->getChild(name) != nullptr) 
			return nullptr; // already exists

		File* file = new File();
		file->parent = this;
		file->name = name;
		this->children.insert(std::make_pair(name, file));
		return file;
	}

	File* Directory::findFile(std::string name)
	{
		Node* node = this->getChild(name);
		File* file = dynamic_cast<File*>(node);
		return file == NULL ? nullptr : file;
	}

	Directory* Directory::createDirectory(std::string name)
	{
		if (name.empty())
			return nullptr; // empty name

		if (name.find(FileSystem::PathSeparator) != std::string::npos)
			return nullptr; // invalid char (backslash)

		if (name.find(":") != std::string::npos && this->parent != nullptr)
			return nullptr; // invalid char (semicolon) - directory is not drive

		if (name.find(":") == std::string::npos && this->parent == nullptr)
			return nullptr; // missing semicolon - drives

		if (this->getChild(name) != nullptr) 
			return nullptr; // already exists

		Directory* dir = new Directory();
		dir->parent = this;
		dir->name = name;
		this->children.insert(std::make_pair(name, dir));
		return dir;
	}

	Directory* Directory::findDirectory(std::string name)
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
		File* file = this->findFile(name);
		if (file == nullptr)
			return S_FALSE;

		delete file;
		return S_OK;
	}

	HRESULT Directory::deleteDirectory(std::string name)
	{
		Directory* dir = this->findDirectory(name);
		if (dir == nullptr)
			return S_FALSE;

		delete dir;
		return S_OK;
	}

	HRESULT Directory::moveChild(std::string name, Directory* dstdir, std::string dstname)
	{
		if (dstname.empty()) 
			return S_FALSE; // dstname is empty

		if (dstdir == this && dstname == name) 
			return S_OK; // src = dst

		if (dstdir->getChild(dstname) != nullptr) 
			return S_FALSE; // dstname already exists in dst directory

		Node* node = this->getChild(name); // find src node
		if (node == nullptr)
			return S_FALSE; // node not found

		this->children.erase(name); // delete from src directory

		node->parent = dstdir; // set new parent
		node->name = dstname; // set new name
		dstdir->children.insert(std::make_pair(dstname, node)); // insert into dst directory

		return S_OK;
	}


}