
#include "LoggerFactory.h"


#include <iostream>
#include "Log.h"
#include "Logger.h"
#include "..\Utils\StringUtils.h"

using namespace std;

namespace Bomberman {
	class StdOutLogger : public Logger {
	protected:
		void recieveLog(string text, LogLevel level) {
			cout << "[" << level.toString() << "]: " << text << endl;
		}
	};
	
	LoggerFactory& LoggerFactory::get() {
		return (*GetInstance());
	}

	shared_ptr<Logger> LoggerFactory::getLogger(string loggerName) {
		shared_ptr<Logger> logger;

		loggerName = StringUtils::toUpper(loggerName);

		if ("STDOUT" == loggerName) {
			if (!loggers.stdOut) {
				loggers.stdOut.reset(new StdOutLogger());
			}

			logger = loggers.stdOut;
		}
		else {
			Log::get() << "Invalid log name: \"" << loggerName << "\"." << LogLevel::warning;
		}

		return logger;
	}
}
