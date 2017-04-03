#pragma once

#include <string>
#include <vector>

namespace Bomberman {
	namespace StringUtils {
		std::vector<std::string> split(std::string str, char separator);

		std::string join(std::vector<std::string> strs, char separator);

		std::string join(std::vector<std::string> strs, std::string separator);

		std::string leftTrim(std::string str);

		std::string rightTrim(std::string str);

		std::string trim(std::string str);

		std::string toLower(std::string str);

		std::string toUpper(std::string str);

		bool tryParseInt(std::string str, int& result);
	}
}