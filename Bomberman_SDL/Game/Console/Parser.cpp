
#include "Parser.h"
#include "../../Core/Utils/Exception.h"
#include "../../Core/Log/Log.h"
#include "First.h"
#include "../CommandFactory.h"
#include "../../Core/Utils/Pointer.h"

using namespace std;

namespace Bomberman {
	bool inSet(set<Token> tokenSet, Token token) {
		return tokenSet.find(token) != tokenSet.end();
	}

	Parser::Parser(weak_ptr<CommandFactory> commandFactory) : commandFactory(commandFactory) {

	}

	Parser::~Parser() {

	}

	queue<shared_ptr<Command>> Parser::parse(string command) {
		commands = queue<shared_ptr<Command>>();

		if (!command.empty()) {
			tokenFactory << command;

			try {
				expressions();
			}
			catch (ParsingErrorException&) {
				Log::get() << "Unexpected token found: \"" << tokenFactory.currentToken().toString() << "\"." << LogLevel::error;

				tokenFactory.clear();
			}
		}

		return commands;
	}

	void Parser::expressions() {
		while (!tokenFactory.empty()) {
			if (inSet(First::expression(), tokenFactory.currentToken())) {
				expression();
			}
			else {
				parsingError();
			}
		}
	}

	void Parser::expression() {
		objectCall = false;
		values.clear();

		call();

		end();

		if (!values.empty()) {
			vector<string> arguments;
			shared_ptr<CommandFactory> commandFactory;

			if (lockWeakPointer(this->commandFactory, commandFactory)) {
				if (objectCall) {
					arguments = vector<string>(values.begin() + 2, values.end());

					try {
						auto command = commandFactory->sendMessage(values[0], values[1], arguments);

						commands.push(command);
					}
					catch (InvalidReceiverException&) {
						Log::get() << "Unknown object \"" << values[0] << "\"." << LogLevel::error;
					}
				}
				else {
					arguments = vector<string>(values.begin() + 1, values.end());

					try {
						auto command = commandFactory->call(values[0], arguments);

						commands.push(command);
					}
					catch (InvalidFunctionException&) {
						Log::get() << "Unkown function \"" << values[0] << "\"." << LogLevel::error;
					}
				}
			}
			else {
				Log::get() << "No CommandFactory for Parser" << LogLevel::error;
			}
		}
	}

	void Parser::call() {
		if (tokenFactory.currentToken() != Token::identifier) {
			return;
		}

		values.push_back(tokenFactory.lexeme());
		tokenFactory.popToken();

		if (inSet(First::object(), tokenFactory.currentToken())) {
			object();
		}
		else if (inSet(First::function(), tokenFactory.currentToken())) {
			function();
		}
		else {
			parsingError();
		}
	}

	void Parser::object() {
		if (tokenFactory.currentToken() == Token::point) {
			tokenFactory.popToken();
		}
		else {
			parsingError();
		}

		if (tokenFactory.currentToken() == Token::identifier) {
			values.push_back(tokenFactory.lexeme());
			tokenFactory.popToken();
		}
		else {
			parsingError();
		}

		objectCall = true;

		function();
	}

	void Parser::function() {
		if (tokenFactory.currentToken() == Token::lPar) {
			tokenFactory.popToken();
		}
		else {
			parsingError();
		}

		argumentList();

		if (tokenFactory.currentToken() == Token::rPar) {
			tokenFactory.popToken();
		}
		else {
			parsingError();
		}
	}

	void Parser::argumentList() {
		if (!inSet(First::constant(), tokenFactory.currentToken())) {
			return;
		}
		constant();

		while (tokenFactory.currentToken() == Token::comma) {
			tokenFactory.popToken();

			constant();
		}
	}

	void Parser::constant() {
		if (inSet(First::constant(), tokenFactory.currentToken())) {
			values.push_back(tokenFactory.lexeme());
			tokenFactory.popToken();
		}
		else {
			parsingError();
		}
	}

	void Parser::end() {
		while (inSet(First::end(), tokenFactory.currentToken()) && !tokenFactory.empty()) {
			tokenFactory.popToken();
		}
	}

	void Parser::parsingError() {
		throw ParsingErrorException();
	}
}
