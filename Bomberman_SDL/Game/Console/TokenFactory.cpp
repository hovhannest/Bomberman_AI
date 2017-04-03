
#include "TokenFactory.h"
#include "Token.h"

using namespace std;

namespace Bomberman {
	TokenFactory::TokenFactory() : cToken(Token::eof), nToken(Token::eof), processing(false) {

	}

	void TokenFactory::push(string str) {
		if (str.empty()) {
			return;
		}

		bool wasEmpty = buffer.empty();

		buffer += str;

		if (wasEmpty && cToken == Token::eof) {
			processToken();
			popToken();
		}
	}

	Token TokenFactory::currentToken() const {
		return cToken;
	}

	string TokenFactory::lexeme() const {
		return cLexeme;
	}

	Token TokenFactory::nextToken() const {
		return nToken;
	}

	bool TokenFactory::empty() const {
		return cToken == Token::eof && nToken == Token::eof && buffer.empty();
	}

	void TokenFactory::clear() {
		cToken = nToken = Token::eof;
		cLexeme.clear();
		nLexeme.clear();
		buffer.clear();
	}

	void TokenFactory::popToken() {
		cToken = nToken;
		cLexeme = nLexeme;

		processToken();
	}

	TokenFactory& TokenFactory::operator<<(string str) {
		push(str);

		return *this;
	}

	void TokenFactory::processToken() {
		enum {
			start,
			id,
			number,
			end,
			error
		} state = start;
		bool done = false;
		nLexeme.clear();

		while (!done) {
			switch (state) {
			case start:
				if (buffer.empty()) {
					nToken = Token::eof;
					state = end;
				}
				else if (nextSymbol() == ' ') {
					popSymbol();
				}
				else if (nextSymbol() == '\n') {
					popSymbol();

					nToken = Token::eol;
					state = end;
				}
				else if (nextSymbol() == '(') {
					nLexeme = nextSymbol();
					popSymbol();

					nToken = Token::lPar;
					state = end;
				}
				else if (nextSymbol() == ')') {
					nLexeme = nextSymbol();
					popSymbol();

					nToken = Token::rPar;
					state = end;
				}
				else if (nextSymbol() == '.') {
					nLexeme = nextSymbol();
					popSymbol();

					nToken = Token::point;
					state = end;
				}
				else if (nextSymbol() == ',') {
					nLexeme = nextSymbol();
					popSymbol();

					nToken = Token::comma;
					state = end;
				}
				else if (nextSymbol() == ';') {
					nLexeme = nextSymbol();
					popSymbol();

					nToken = Token::stop;
					state = end;
				}
				else if (isdigit(nextSymbol())) {
					nLexeme = nextSymbol();
					popSymbol();

					state = number;
				}
				else if (isalpha(nextSymbol()) || nextSymbol() == '_') {
					nLexeme = nextSymbol();
					popSymbol();

					state = id;
				}
				else {
					state = error;
				}
				break;

			case id:
				if (isalnum(nextSymbol()) || nextSymbol() == '_') {
					nLexeme += nextSymbol();
					popSymbol();
				}
				else {
					nToken = Token::identifier;
					state = end;
				}
				break;

			case number:
				if (isdigit(nextSymbol())) {
					nLexeme += nextSymbol();
					popSymbol();
				}
				else if (isalpha(nextSymbol())) {
					state = error;
				}
				else {
					nToken = Token::number;
					state = end;
				}
				break;

			case end:
				done = true;
				break;

			default:
				nToken = Token::error;
				state = end;
				break;
			}
		}
	}

	char TokenFactory::nextSymbol() {
		return buffer[0];
	}

	void TokenFactory::popSymbol() {
		buffer.erase(0, 1);
	}
}
