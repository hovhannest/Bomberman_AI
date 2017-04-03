#pragma once

#include "ScreenComponent.h"

namespace Bomberman {
	enum class Signal;

	class SignalHandler : public ScreenComponent {
	public:
		virtual ~SignalHandler() {}

		virtual void handleSignal(Signal signal) = 0;
	};
}