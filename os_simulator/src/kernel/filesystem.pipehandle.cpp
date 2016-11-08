#include "filesystem.PipeHandle.h"


namespace FileSystem {

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
		std::unique_lock<std::mutex> lck(mtx);
		const char* str = static_cast<const char*>(buffer);

		int i = 0;

		// TODO: zavreni pipy - nejak lepe? (staci, aby kdokoliv zapsal 26, potom read funguje, dokud je neco v bufferu; write vraci 0)
		while(i < buffer_size && pipeOpened) {
			if (str[i] == 26) {
				pipeOpened = false;
				break;
			}

			this->queue.push(str[i]);
			i++;
		}

		cv.notify_all();

		if (pwritten != nullptr)
			*pwritten = i;
	}

	void PipeHandle::read(char** buffer, const size_t buffer_size, size_t* pread)
	{
		std::unique_lock<std::mutex> lck(mtx);

		int i = 0;
		while(i < buffer_size) {
			if (this->queue.empty()) {
				if (!pipeOpened) break;
				cv.wait(lck);
				continue; // recheck emptiness
			}

			(*buffer)[i++] = this->queue.front();
			this->queue.pop();
		}

		if (pread != nullptr)
			*pread = i;
	}

	void PipeHandle::close()
	{
		delete this;
	}

}