
#include "OperatingSystem.h"

#include <memory>

#if defined(_WIN32)
#include <Windows.h>
#endif

using namespace std;

namespace Bomberman {
	string getPath(vector<string> dirs) {
		string result;

		for (size_t i = 0; i < dirs.size(); ++i) {
			result += dirs[i] + dirSeparator;
		}

		return result;
	}

	string getPath(vector<string> dirs, string file) {
		return getPath(dirs) + file;
	}

	bool listDir(string path, vector<string>& contents) {
		bool result = false;

#if defined(_WIN32)
		path = getPath({ path }, "*");

		HANDLE directoryHandle = INVALID_HANDLE_VALUE;
		WIN32_FIND_DATAA data;

		directoryHandle = FindFirstFileA(path.c_str(), &data);
		if (INVALID_HANDLE_VALUE != directoryHandle) {
			do {
				contents.push_back(data.cFileName);
			} while (FindNextFileA(directoryHandle, &data));

			result = true;
		}
#endif

		return result;
	}
}