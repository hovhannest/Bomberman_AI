#pragma once

#include <memory>
#include <sstream>
#include <vector>

#include "LogLevel.h"
#include "..\Utils\Singleton.h"

namespace Bomberman {
	class Logger;

	class Log :
		public Singleton<Log>
	{
	public:
		static Log& get();

		Log& operator<<(int value);
		Log& operator<<(long int value);
		Log& operator<<(const char *value);
		Log& operator<<(std::string value);
		Log& operator<<(void *value);
		void operator<<(LogLevel level);
		void operator<<(std::exception exception);

		void addLogger(std::shared_ptr<Logger> logger);

	private:
		void flush(LogLevel level);

		std::stringstream buffer;
		std::vector<std::pair<std::string, LogLevel>> messages;
		std::shared_ptr<Logger> logger;
	};
}