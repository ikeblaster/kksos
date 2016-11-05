#pragma once
#include <sstream> 
#include "filesystem.h"


namespace FileSystem {
	namespace Utils {

		class FileHandle {
			private:
			File* file;
			std::stringstream ss;

			public:
			FileHandle(File* file, bool newFile = true);
			void write(const void* buffer, const size_t buffer_size, size_t* written);
			fpos_t seek(const fpos_t pos, std::ios_base::seekdir way);
			fpos_t tell();
			void read(char** buffer, const size_t buffer_size, size_t* read);
			void close();
		};

		class Path {
			public:
			static RESULT parse(Directory* cwd, std::string path, Directory** parsedDirectory, File** parsedFile);
			static std::string getBasename(std::string path);
			static Directory* getDriveRoot(Directory* directory);
			static std::string generate(Node* node);

			private:
			static std::string trimTrailingSeparators(std::string path);
		};
		
	}
}
