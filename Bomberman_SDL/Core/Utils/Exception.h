#pragma once

#include <exception>

#if defined(_WIN32)
// Visual Studio doesn't have noexcept
// should use a better macro name...
#define noexcept
#endif

namespace Bomberman {
	class Exception : public std::exception {
	public:

	};

	class NotImplementedException : public Exception {
	public:
		virtual const char* what() const noexcept {
			return "action not implemented";
		}
	};

	class FatalException : public Exception {
	public:
		virtual const char* what() const noexcept {
			return "fatal error";
		}
	};

	class InvalidOperationException : public Exception {
	public:
		virtual const char* what() const noexcept {
			return "invalid operation";
		}
	};

	class OpeningFileErrorException : public Exception {
	public:
		virtual const char* what() const noexcept {
			return "error opening a file";
		}
	};

	class ParsingErrorException : public Exception {
	public:
		virtual const char* what() const noexcept {
			return "invalid value while parsing a string";
		}
	};

	class InvalidReceiverException : public Exception {
	public:
		virtual const char* what() const noexcept {
			return "invalid receiver for a message";
		}
	};

	class OutOfRangeException : public Exception {
	public:
		virtual const char* what() const noexcept {
			return "value out of range";
		}
	};

	class NullArgumentException : public Exception {
	public:
		virtual const char* what() const noexcept {
			return "argument is null";
		}
	};

	class InvalidArgumentException : public Exception {
	public:
		virtual const char* what() const noexcept {
			return "argument is invalid";
		}
	};

	class InvalidFunctionException : public Exception {
	public:
		virtual const char* what() const noexcept {
			return "invalid function";
		}
	};

	class NativeOperationException : public Exception {
	public:
		virtual const char* what() const noexcept {
			return "error executing native code";
		}
	};
}