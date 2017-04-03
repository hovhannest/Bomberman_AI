#pragma once

namespace Bomberman {
	enum class Signal;

	class SignalSender {
	public:
		virtual ~SignalSender() {}

		virtual void sendSignal(Signal signal) = 0;
	};
}
