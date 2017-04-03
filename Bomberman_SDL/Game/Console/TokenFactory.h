#pragma once

#include <string>

#include "Token.h"

namespace Bomberman {
	class TokenFactory {
	public:
		TokenFactory();

		void push(std::string string);

		std::string lexeme() const;

		Token currentToken() const;
		Token nextToken() const;

		bool empty() const;
		void clear();

		void popToken();
		TokenFactory& operator<<(std::string string);

	private:
		void processToken();

		char nextSymbol();
		void popSymbol();

		bool processing;
		Token cToken, nToken;
		std::string cLexeme, nLexeme;
		std::string buffer;
	};
}