#include "filesystem.PipeHandle.h"


namespace FileSystem {

	fpos_t PipeHandle::seek(const fpos_t pos, std::ios_base::seekdir way)
	{
		if (way == std::ios_base::beg + std::ios_base::end) {
			std::unique_lock<std::mutex> lck(mtx);
			pipeOpened = false;
			cv.notify_all();
		}
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

	void PipeHandle::read(char** buffer, const size_t buffer_size, size_t* pread)
	{
		std::unique_lock<std::mutex> lck(mtx);

		int i = 0;
		while(i < buffer_size) {
			if (size == 0) {
				if (!pipeOpened) break;

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

	void PipeHandle::close()
	{
		delete this;
	}

}