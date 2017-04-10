#pragma once

#include "..\..\Core\EventListener.h"
#include "..\..\Core\SignalHandler.h"
#include <memory>

namespace Bomberman {
	class CommandFactory;
	class CommandQueue;

	class AIEvents : public EventListener, public SignalHandler {
	public:
		AIEvents();

		void listenEvent(SDL_Event event);
		void handleSignal(Signal signal);

		void setCommandFactory(std::weak_ptr<CommandFactory> commandFactory);
		void setCommandQueue(std::weak_ptr<CommandQueue> commandQueue);

	private:
		std::weak_ptr<CommandFactory> commandFactory;
		std::weak_ptr<CommandQueue> commandQueue;
	};
}