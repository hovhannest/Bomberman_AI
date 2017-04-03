#pragma once

#include <string>

namespace Bomberman {
	class Token {
	public:
		static const Token error;
		static const Token eof;
		static const Token eol;
		static const Token stop;
		static const Token lPar;
		static const Token rPar;
		static const Token identifier;
		static const Token number;
		static const Token point;
		static const Token comma;

		Token();
		Token(const Token& other);

		int value() const;
		std::string toString() const;

		bool operator==(const Token& other) const;
		bool operator!=(const Token& other) const;
		bool operator<(const Token& other) const;
		bool operator>(const Token& other) const;
		bool operator<=(const Token& other) const;
		bool operator>=(const Token& other) const;

	private:
		Token(int value, std::string str);
		int _value;
		std::string str;
	};
}