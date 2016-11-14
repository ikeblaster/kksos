#pragma once
#include <sstream> 
#include "../common/api.h"
#include "filesystem.h"


namespace FileSystem {

	class Path {
		public:
		static RESULT parse(Directory* cwd, std::string path, Directory** parsedDirectory, File** parsedFile, flags_t flags);
		static std::string getBasename(std::string path);
		static Directory* getDriveRoot(Directory* directory);
		static std::string generate(Node* node);

		private:
		static std::string trimTrailingSeparators(std::string path);
	};

}
