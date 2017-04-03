#pragma once

#include <memory>
#include <string>

namespace Bomberman {
	class LogLevel;

	class Logger {
		friend class Log;
	public:
		virtual ~Logger() {};

	protected:
		virtual void recieveLog(std::string text, LogLevel level) = 0;

	private:
		std::shared_ptr<Logger> next;
	};
}