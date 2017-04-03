
#include "LogLevel.h"

namespace Bomberman {
	LogLevel::LogLevel(int type, std::string str) : type(type), str(str) {

	}

	std::string LogLevel::toString() const {
		return str;
	}

	bool LogLevel::operator==(const LogLevel& other) const {
		return type == other.type;
	}

	bool LogLevel::operator!=(const LogLevel& other) const {
		return !(*this == other);
	}

	const LogLevel LogLevel::info = LogLevel(0, "info");
	const LogLevel LogLevel::warning = LogLevel(1, "warning");
	const LogLevel LogLevel::error = LogLevel(2, "error");
	const LogLevel LogLevel::fatal = LogLevel(3, "fatal");
}