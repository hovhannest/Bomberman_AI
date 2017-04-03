
#include "First.h"

#include <vector>

#include "Token.h"

using namespace std;

namespace Bomberman {
	set<Token> setUnion(vector<set<Token>> sets) {
		set<Token> result;

		for (auto s = sets.begin(); s != sets.end(); ++s) {
			result.insert(s->begin(), s->end());
		}

		return result;
	}

	set<Token> First::expressions() {
		return expression();
	}

	set<Token> First::expression() {
		return setUnion({ call(), end() });
	}


	set<Token> First::call() {
		return{ Token::identifier };
	}

	set<Token> First::object() {
		return{ Token::point };
	}

	set<Token> First::function() {
		return{ Token::lPar };
	}

	set<Token> First::argumentList() {
		return constant();
	}

	set<Token> First::constant() {
		return{ Token::identifier, Token::number };
	}

	set<Token> First::end() {
		return{ Token::eof, Token::eol, Token::stop };
	}
}