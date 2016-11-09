#include "filesystem.pipe.h"


namespace FileSystem {

	Pipe::Pipe()
	{
		this->pipeReadable = new PipeHandle(PIPETYPE::READABLE, this); // process writes into this
		this->pipeWriteable = new PipeHandle(PIPETYPE::WRITEABLE, this); // process reads from this
	}

	Pipe::~Pipe()
	{
		delete this->pipeReadable;
		delete this->pipeWriteable;
	}

	void Pipe::closePipeHandle(PIPETYPE type)
	{
		pipeOpened = false;

		if (type == PIPETYPE::READABLE) this->pipeReadable = nullptr;
		if (type == PIPETYPE::WRITEABLE) this->pipeWriteable = nullptr;

		if (this->pipeReadable == nullptr && this->pipeWriteable == nullptr) 
			this->close();
		else
			cv.notify_all();
	}

	PipeHandle* Pipe::getPipeHandle(PIPETYPE type) 
	{
		if (type == PIPETYPE::READABLE) return this->pipeReadable;
		if (type == PIPETYPE::WRITEABLE) return this->pipeWriteable;
		return nullptr;
	}

	fpos_t Pipe::seek(const fpos_t pos, std::ios_base::seekdir way)
	{
		return 0;
	}

	fpos_t Pipe::tell()
	{
		return 0;
	}

	void Pipe::write(const void* buffer, const size_t buffer_size, size_t* pwritten)
	{
		std::unique_lock<std::mutex> lck(mtx);
		const char* str = static_cast<const char*>(buffer);

		int i = 0;

		while(i < buffer_size && pipeOpened) {
			if (str[i] == 26) {
				pipeOpened = false; // nobody wants to read from pipe anymore -> skip writing
				break;
			}

			if(size == MAX_BUFFER_SIZE) {
				//printf("W");
				cv.notify_all();
				cv.wait(lck);
				continue;
			}

			data_buffer[last] = str[i];
			last = (last + 1) % MAX_BUFFER_SIZE;
			size++;
			i++;
		}

		cv.notify_all();

		if (pwritten != nullptr)
			*pwritten = i;
	}

	void Pipe::read(char** buffer, const size_t buffer_size, size_t* pread)
	{
		std::unique_lock<std::mutex> lck(mtx);

		int i = 0;
		while(i < buffer_size) {
			if (size == 0) {
				if (!pipeOpened) break; // nobody wants to write to pipe anymore -> don't wait for another data

				//printf("R");
				cv.notify_all();
				cv.wait(lck);
				continue; // recheck emptiness
			}

			(*buffer)[i++] = data_buffer[first];
			//nastavit data_buffer[first] na null, nebo nechat byt a usetrit rezii?
			first = (first + 1) % MAX_BUFFER_SIZE;	
			size--;
		}
		cv.notify_all();

		if (pread != nullptr)
			*pread = i;
	}

	void Pipe::close()
	{
		delete this;
	}

}