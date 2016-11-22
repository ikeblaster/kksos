#pragma once
#include <sstream> 
#include "../common/api.h"
#include "filesystem.h"


namespace FileSystem {

	class Path {
		public:
		/// <summary>
		/// Parses specified path above file system.
		/// </summary>
		/// <param name="cwd">Current working dir (starting point).</param>
		/// <param name="path">Path to file/directory.</param>
		/// <param name="parsedDirectory">[out] Pointer to parsed directory object.</param>
		/// <param name="parsedFile">[out] Pointer to parsed file object.</param>
		/// <param name="flags">Flags for parsing.</param>
		/// <returns>Result.</returns>
		static RESULT parse(Directory* cwd, std::string path, Directory** parsedDirectory, File** parsedFile, flags_t flags);

		/// <summary>
		/// Gets basename of path (last part of path).
		/// </summary>
		/// <param name="path">Path.</param>
		/// <returns>Basename of path.</returns>
		static std::string getBasename(std::string path);

		/// <summary>
		/// Gets drive root for directory.
		/// </summary>
		/// <param name="directory">Directory object.</param>
		/// <returns>Directory object to drive root.</returns>
		static Directory* getDriveRoot(Directory* directory);

		/// <summary>
		/// Generates path (text representation) from file/directory object.
		/// </summary>
		/// <param name="node">File/directory object.</param>
		/// <returns>Path.</returns>
		static std::string generate(Node* node);


		private:
		/// <summary>
		/// Trims trailing path separators from path.
		/// </summary>
		/// <param name="path">Path.</param>
		/// <returns>Path without trailing separators.</returns>
		static std::string trimTrailingSeparators(std::string path);
	};

}
