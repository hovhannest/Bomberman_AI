#pragma once

#include <memory>
#include <string>
#include "..\Utils\Singleton.h"

namespace Bomberman {
	class Logger;

	class LoggerFactory :
		public Singleton<LoggerFactory>
	{
	public:
		static LoggerFactory& get();

		std::shared_ptr<Logger> getLogger(std::string loggerName);

	private:

		struct {
			std::shared_ptr<Logger> stdOut;
		} loggers;

	};
}
