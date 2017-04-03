#pragma once

#include <string>
#include <vector>

namespace Bomberman {

#if defined(_WIN32)
	const char dirSeparator = '\\';
#else
	const char dirSeparator = '/';
#endif

	std::string getPath(std::vector<std::string> dirs);
	std::string getPath(std::vector<std::string> dirs, std::string file);

	bool listDir(std::string path, std::vector<std::string>& contents);
}