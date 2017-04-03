
#include "Token.h"

using namespace std;

namespace Bomberman {
	Token::Token() : _value(eof._value), str(eof.str) {

	}

	Token::Token(const Token& other) : _value(other._value), str(other.str) {

	}

	Token::Token(int value, string str) : _value(value), str(str) {

	}

	int Token::value() const {
		return _value;
	}

	std::string Token::toString() const {
		return str;
	}

	bool Token::operator==(const Token& other) const {
		return _value == other._value;
	}

	bool Token::operator!=(const Token& other) const {
		return !(*this == other);
	}

	bool Token::operator<(const Token& other) const {
		return _value < other._value;
	}

	bool Token::operator>(const Token& other) const {
		return other < *this;
	}

	bool Token::operator<=(const Token& other) const {
		return !(*this > other);
	}

	bool Token::operator>=(const Token& other) const {
		return !(*this < other);
	}

	const Token Token::error = Token(-1, "error");
	const Token Token::eof = Token(0, "eof");
	const Token Token::eol = Token(1, "eol");
	const Token Token::stop = Token(2, "stop");
	const Token Token::lPar = Token(3, "lPar");
	const Token Token::rPar = Token(4, "rPar");
	const Token Token::identifier = Token(5, "identifier");
	const Token Token::number = Token(6, "number");
	const Token Token::point = Token(7, "point");
	const Token Token::comma = Token(8, "comma");
}
