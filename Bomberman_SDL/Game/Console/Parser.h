#pragma once

#include <memory>
#include <queue>
#include <set>

#include "../../Core/Command.h"
#include "TokenFactory.h"

namespace Bomberman {
	class CommandFactory;

	class Parser {
	public:
		Parser(std::weak_ptr<CommandFactory> commandFactory);
		~Parser();

		std::queue<std::shared_ptr<Command>> parse(std::string command);

	private:
		std::weak_ptr<CommandFactory> commandFactory;
		std::queue<std::shared_ptr<Command>> commands;

		void expressions();

		void expression();

		void call();

		void object();

		void function();

		void argumentList();

		void constant();

		void ends();

		void end();

		void endsOpt();

		void parsingError();

		TokenFactory tokenFactory;

		bool objectCall;
		std::vector<std::string> values;
	};
}
