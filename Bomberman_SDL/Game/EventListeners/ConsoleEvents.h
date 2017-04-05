#pragma once

#include <memory>
#include "../../Core/EventListener.h"
#include "../../Core/SignalHandler.h"

namespace Bomberman {
	class CommandQueue;
	class Console;

	class ConsoleEvents : public EventListener, public SignalHandler {
	public:
		void listenEvent(SDL_Event event);

		void setConsole(std::weak_ptr<Console> console);

		void handleSignal(Signal signal);

	private:
		std::weak_ptr<Console> console;
	};
}