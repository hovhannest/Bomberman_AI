
#include "Signal.h"

namespace Bomberman
{

	bool signalIn(Signal signal, std::initializer_list<Signal> signals) {
		for (auto otherSignal : signals) {
			if (otherSignal == signal) {
				return true;
			}
		}

		return false;
	}
}