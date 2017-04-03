
#include "StringUtils.h"


#include <sstream>
#include <algorithm>
#include <ctype.h>

using namespace std;

namespace Bomberman {
	namespace StringUtils {
		vector<string> split(string str, char separator) {
			vector<string> result;
			stringstream buffer;

			for (int n = 0; n < (int)(str.length()); ++n) {
				if (str[n] == separator) {
					result.push_back(buffer.str());

					buffer.str(string());
					buffer.clear();
				}
				else {
					buffer << str[n];
				}
			}

			if (!str.empty()) {
				result.push_back(buffer.str());
			}

			return result;
		}

		string join(vector<string> strs, char separator) {
			return join(strs, string(1, separator));
		}

		string join(vector<string> strs, std::string separator) {
			stringstream buffer;

			for (int n = 0; n < (int)(strs.size()); ++n) {
				if (n != 0) {
					buffer << separator;
				}

				buffer << strs[n];
			}

			return buffer.str();
		}

		string leftTrim(string str) {
			size_t position = str.find_first_not_of(" \t");

			if (position == string::npos) {
				return string();
			}

			return str.substr(position);
		}

		string rightTrim(string str) {
			size_t position = str.find_last_not_of(" \t");

			if (position == string::npos) {
				return string();
			}

			return str.substr(0, position + 1);
		}

		string trim(string str) {
			return leftTrim(rightTrim(str));
		}

		string toLower(string str) {
			string result;

			for (char c : str) {
				result += tolower(c);
			}

			return result;
		}

		string toUpper(string str) {
			string result;

			for (char c : str) {
				result += toupper(c);
			}

			return result;
		}

		bool tryParseInt(string str, int& result) {
			try {
				result = atoi(str.c_str());
			}
			catch (logic_error) {
				return false;
			}

			return true;
		}
	}
}
