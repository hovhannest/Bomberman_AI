
#include "CommandQueue.h"

#include "Command.h"
#include "Log\Log.h"
#include "Log\LogLevel.h"

using namespace std;

namespace Bomberman {
	void CommandQueue::addCommand(shared_ptr<Command> command) {
		if (!command) {
			Log::get() << "Inserting empty command." << LogLevel::error;

			return;
		}

		commands.push(command);
	}

	void CommandQueue::update() {
		if (commands.empty()) {
			return;
		}

		if (commands.front()->finished()) {
			commands.pop();
		}
		else {
			commands.front()->execute();
		}
	}

	void CommandQueue::clear() {
		queue<shared_ptr<Command>> empty;
		swap(commands, empty);
	}
}