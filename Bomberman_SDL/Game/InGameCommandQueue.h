#pragma once

#include "../Core/CommandQueue.h"
#include "../Core/SignalHandler.h"
#include "../Core/SignalSender.h"
#include "Signal.h"

namespace Bomberman {
	class InGameCommandQueue : public CommandQueue, public SignalHandler {
	public:
		void handleSignal(Signal signal) {
			if (signalIn(signal, { Signal::InGame, Signal::ShowConsole, Signal::HideConsole, Signal::PlayerAlive, Signal::PlayerDead })) {
				Updatable::enable();
			}
			else {
				Updatable::disable();
			}
		}
	};
}