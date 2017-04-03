#pragma once

#include <memory>
#include <queue>

#include "Updatable.h"

namespace Bomberman {
	class Command;

	class CommandQueue : public Updatable {
	public:
		void addCommand(std::shared_ptr<Command> command);

		void update();

		void clear();

	private:
		std::queue<std::shared_ptr<Command>> commands;
	};
}