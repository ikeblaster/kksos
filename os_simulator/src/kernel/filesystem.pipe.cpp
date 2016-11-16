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

	void Pipe::close()
	{
		delete this;
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

	void Pipe::write(const void* buffer, const size_t buffer_size, size_t* pwritten)
	{
		std::unique_lock<std::mutex> lck(mtx);
		const char* str = static_cast<const char*>(buffer);

		size_t i = 0;

		while(i < buffer_size && pipeOpened) { // pipe closed = no reader -> skip writing
			if (str[i] == CHAR_EOF) {
				pipeOpened = false; // EOF came -> close pipe
				break;
			}

			if(size == MAX_BUFFER_SIZE) {
				cv.notify_all();
				cv.wait(lck);
				continue;
			}

			size_t ncopy = MAX_BUFFER_SIZE - size;
			if (ncopy > (buffer_size - i)) ncopy = (buffer_size - i);
			if (last + ncopy > MAX_BUFFER_SIZE) ncopy = MAX_BUFFER_SIZE - last;

			memcpy(&data_buffer[last], &str[i], ncopy);

			i += ncopy;
			size += ncopy;
			last = (last + ncopy) % MAX_BUFFER_SIZE;
		}

		cv.notify_all();

		if (pwritten != nullptr)
			*pwritten = i;
	}

	void Pipe::read(char* buffer, const size_t buffer_size, size_t* pread)
	{
		std::unique_lock<std::mutex> lck(mtx);

		size_t i = 0;
		while(i < buffer_size) {
			if (size == 0) {
				if (!pipeOpened) break; // no data in buffer + closed pipe -> don't wait for another data

				cv.notify_all();
				cv.wait(lck);
				continue; // recheck emptiness
			}

			size_t ncopy = buffer_size - i;
			if (ncopy > size) ncopy = size;
			if (first + ncopy > MAX_BUFFER_SIZE) ncopy = MAX_BUFFER_SIZE - first;

			memcpy(&buffer[i], &data_buffer[first], ncopy);

			i += ncopy;
			size -= ncopy;
			first = (first + ncopy) % MAX_BUFFER_SIZE;
		}
		cv.notify_all();

		if (pread != nullptr)
			*pread = i;
	}

}
