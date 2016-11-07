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

		for (int i = 0; i < buffer_size && !writeClosed; i++) {
			this->queue.push(str[i]); // TODO: nic nezapisovat, pokud stdout je zavreny - jak zjistit?
			if (str[i] == 26)
				writeClosed = true;
		}

		cv.notify_all();

		if (pwritten != nullptr)
			*pwritten = buffer_size;
	}

	void PipeHandle::read(char** buffer, const size_t buffer_size, size_t* pread)
	{
		std::unique_lock<std::mutex> lck(mtx);

		int i = 0;
		while(i < buffer_size && !readClosed) {
			while (this->queue.empty()) 
				cv.wait(lck);

			if (this->queue.front() == 26) { // TODO: nejak lepe?
				readClosed = true;
				break;
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