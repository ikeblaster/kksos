#pragma once
#include <sstream> 
#include "filesystem.h"

#define OPEN_EXISTING 3

namespace FileSystem {

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
