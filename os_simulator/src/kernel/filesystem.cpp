#include "filesystem.h"

std::string stringtolower(std::string string) {
	std::transform(string.begin(), string.end(), string.begin(), ::tolower);
	return string;
}

namespace FileSystem {

	FileSystem::Directory* fs = nullptr;
	FileSystem::Directory* fs_root = nullptr;

	// === Node members

	Node::~Node()
	{
		if (this->parent != nullptr)
			this->parent->children.erase(stringtolower(this->name));
	}

	Directory* Node::getParent()
	{
		return this->parent;
	}

	std::string Node::getName()
	{
		return this->name;
	}

	std::string Node::toString()
	{
		return this->getName();
	}


	// === File members

	std::string File::getData()
	{
		return this->data;
	}

	void File::setData(std::string data)
	{
		this->data = data;
	}

	RESULT File::destroy()
	{
		if (IO::check_file_open(this)) {
			return RESULT::UNABLE_TO_DELETE;
		}
		delete this;
		return RESULT::OK;
	}



	// === Directory members

	Node* Directory::getChild(std::string name)
	{
		auto search = this->children.find(stringtolower(name));
		if (search != this->children.end()) {
			return search->second;
		}
		return nullptr;
	}

	std::string Directory::toString()
	{
		return this->name + FileSystem::PathSeparator;
	}

	File* Directory::createFile(std::string name)
	{
		if (name.empty()) 
			return nullptr; // empty name

		if (name.find(":") != std::string::npos)
			return nullptr; // invalid char (semicolon)

		if (this->getChild(name) != nullptr) 
			return nullptr; // already exists

		File* file = new File();
		file->parent = this;
		file->name = name;
		this->children.insert(std::make_pair(stringtolower(name), file));
		return file;
	}

	File* Directory::findFile(std::string name)
	{
		Node* node = this->getChild(name);
		File* file = dynamic_cast<File*>(node);
		return file == NULL ? nullptr : file;
	}

	RESULT Directory::deleteFile(std::string name)
	{
		File* file = this->findFile(name);
		if (file == nullptr)
			return RESULT::FILE_NOT_FOUND;

		return file->destroy();
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
		this->children.insert(std::make_pair(stringtolower(name), dir));
		return dir;
	}

	Directory* Directory::findDirectory(std::string name)
	{
		Node* node = this->getChild(name);
		Directory* dir = dynamic_cast<Directory*>(node);
		return dir == NULL ? nullptr : dir;
	}

	RESULT Directory::deleteDirectory(std::string name)
	{
		Directory* dir = this->findDirectory(name);
		if (dir == nullptr)
			return RESULT::DIRECTORY_NOT_FOUND;

		delete dir;
		return RESULT::OK;
	}


	RESULT Directory::moveChild(std::string name, Directory* dstdir, std::string dstname)
	{
		if (dstname.empty()) 
			return RESULT::GENERAL_ERROR; // dstname is empty

		if (dstdir == this && stringtolower(dstname) == stringtolower(name))
			return RESULT::OK; // src = dst

		if (dstdir->getChild(dstname) != nullptr) 
			return RESULT::ALREADY_EXISTS; // dstname already exists in dst directory

		Node* node = this->getChild(name); // find src node
		if (node == nullptr)
			return RESULT::NOT_FOUND; // node not found

		this->children.erase(stringtolower(name)); // delete from src directory

		node->parent = dstdir; // set new parent
		node->name = dstname; // set new name
		dstdir->children.insert(std::make_pair(stringtolower(dstname), node)); // insert into dst directory

		return RESULT::OK;
	}


	std::map<std::string, Node*> Directory::getFiles()
	{
		return this->children;
	}

	RESULT Directory::destroy()
	{
		if (IO::check_directory_open(this)) {
			return RESULT::UNABLE_TO_DELETE;
		}

		while (!this->children.empty()) {
			auto item = *(this->children.begin());

			if (item.second->destroy() != RESULT::OK) {
				return RESULT::UNABLE_TO_DELETE;
			}
		}

		delete this;

		return RESULT::OK;
	}

}