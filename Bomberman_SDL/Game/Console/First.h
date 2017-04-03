#pragma once

#include <set>

namespace Bomberman {
	class Token;

	class First {
	public:
		static std::set<Token> expressions();

		static std::set<Token> expression();

		static std::set<Token> call();

		static std::set<Token> object();

		static std::set<Token> function();

		static std::set<Token> argumentList();

		static std::set<Token> constant();

		static std::set<Token> end();
	};
}