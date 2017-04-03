#pragma once

#include "..\..\Core\Command.h"
#include "..\..\Core\LoopQuiter.h"

#include <memory>

namespace Bomberman {
	class LoopQuiter;

	class QuitCommand : public Command {
	public:
		QuitCommand(std::shared_ptr<LoopQuiter> loopQuiter) {}

		inline void execute() {
			loopQuiter->quitLoop();
			_finished = true;
		}

	private:
		std::shared_ptr<LoopQuiter> loopQuiter;
	};
}