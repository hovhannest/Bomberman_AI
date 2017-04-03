#pragma once

#include <string>

namespace Bomberman {
	class LogLevel {
	public:
		static const LogLevel info;
		static const LogLevel warning;
		static const LogLevel error;
		static const LogLevel fatal;

		std::string toString() const;

		bool operator==(const LogLevel& other) const;
		bool operator!=(const LogLevel& other) const;

	private:
		LogLevel(int type, std::string str);
		int type;
		std::string str;
	};
}