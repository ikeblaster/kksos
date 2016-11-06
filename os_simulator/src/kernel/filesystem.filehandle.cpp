#include "filesystem.filehandle.h"

namespace FileSystem {

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


}