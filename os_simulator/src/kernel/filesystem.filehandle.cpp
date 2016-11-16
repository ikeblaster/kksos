#include "filesystem.filehandle.h"


namespace FileSystem {

	// === members

	FileHandle::FileHandle(File* file, flags_t flags)
	{
		this->file = file;
		
		if((flags & FH_OPEN_EXISTING) == FH_OPEN_EXISTING || (flags & FH_OPEN_OR_CREATE) == FH_OPEN_OR_CREATE)
			this->ss.str(this->file->getData());

		if((flags & FH_FILE_APPEND) == FH_FILE_APPEND)
			this->seek(0, std::ios_base::end);
		else
			this->seek(0, std::ios_base::beg);
	}

	intptr_t FileHandle::getHash()
	{
		return (intptr_t) this->file;
	}

	flags_t FileHandle::probe(flags_t flags)
	{
		return 0;
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
		this->ss.write((const char*)buffer, buffer_size);

		this->ss.clear();
		this->ss.seekg(this->ss.tellp()); // sync positions (get->put)

		if(written != nullptr)
			*written = buffer_size;
	}

	void FileHandle::read(char* buffer, const size_t buffer_size, size_t* read)
	{
		this->ss.clear();
		this->ss.read(buffer, buffer_size);		
		if (read != nullptr)
			*read = this->ss.gcount();
	}

	void FileHandle::close()
	{
		this->ss.clear();
		this->file->setData(this->ss.str());

		delete this;
	}


}