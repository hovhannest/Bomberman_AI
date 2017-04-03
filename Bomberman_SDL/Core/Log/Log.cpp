
#include "Log.h"

#include <iostream>

#include "..\Utils\Exception.h"
#include "Logger.h"

namespace Bomberman {
	
	Log& Log::get() {
		return (*GetInstance());
	}

	Log& Log::operator<<(int value) {
		buffer << value;

		return (*GetInstance());
	}

	Log& Log::operator<<(long int value) {
		buffer << value;

		return (*GetInstance());
	}

	Log& Log::operator<<(const char *value) {
		buffer << value;

		return (*GetInstance());
	}

	Log& Log::operator<<(std::string value) {
		buffer << value;

		return (*GetInstance());
	}

	Log& Log::operator<<(void *value) {
		buffer << value;

		return (*GetInstance());
	}

	void Log::operator<<(LogLevel level) {
		flush(level);

		if (level == LogLevel::fatal) {
			throw FatalException();
		}
	}

	void Log::operator<<(std::exception exception) {
		flush(LogLevel::fatal);

		throw exception;
	}

	void Log::addLogger(std::shared_ptr<Logger> logger) {
		if (!logger) {
			get() << "Adding empty logger." << LogLevel::error;
			return;
		}

		if (this->logger) {
			for (auto current = this->logger; true; current = current->next) {
				if (current == logger) {
					get() << "Adding duplicate logger." << LogLevel::warning;
					return;
				}
				else if (!current->next) {
					current->next = logger;
					break;
				}
			}
		}
		else {
			this->logger = logger;
		}

		for (auto message : messages) {
			logger->recieveLog(message.first, message.second);
		}
	}

	void Log::flush(LogLevel level) {
		std::string message = buffer.str();

		messages.push_back({ message, level });
		for (auto current = logger; current; current = current->next) {
			current->recieveLog(message, level);
		}

		buffer.str(std::string());
		buffer.clear();
	}
}