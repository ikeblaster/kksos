#include "filesystem.PipeHandle.h"


namespace FileSystem {

	PipeHandle::PipeHandle(PIPETYPE type, Pipe* pipe)
	{
		this->type = type;
		this->pipe = pipe;
	}

	intptr_t PipeHandle::getHash()
	{
		return (intptr_t) this;
	}

	flags_t PipeHandle::probe(flags_t flags)
	{
		return 0;
	}

	fpos_t PipeHandle::seek(const fpos_t pos, std::ios_base::seekdir way)
	{
		return 0;
	}

	fpos_t PipeHandle::tell()
	{
		return 0;
	}
		
	void PipeHandle::write(const void* buffer, const size_t buffer_size, size_t* pwritten)
	{
		if (this->type == PIPETYPE::READABLE) {
			if (pwritten != nullptr)
				*pwritten = 0;
		}
		else
			this->pipe->write(buffer, buffer_size, pwritten);
	}

	void PipeHandle::read(char** buffer, const size_t buffer_size, size_t* pread)
	{
		if (this->type == PIPETYPE::WRITEABLE) {
			if (pread != nullptr)
				*pread = 0;
		}
		else
			this->pipe->read(buffer, buffer_size, pread);
	}

	void PipeHandle::close()
	{
		this->pipe->closePipeHandle(this->type);
		delete this;
	}


}