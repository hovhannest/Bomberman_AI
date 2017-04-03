
#include "ScreenComponent.h"


namespace Bomberman {
	ScreenComponent::ScreenComponent() : enabled(true), finished(false) {

	}

	ScreenComponent::~ScreenComponent() {

	}

	bool ScreenComponent::isEnabled() const {
		return enabled;
	}

	void ScreenComponent::enable() {
		enabled = true;
	}

	void ScreenComponent::disable() {
		enabled = false;
	}

	bool ScreenComponent::isFinished() const {
		return finished;
	}

	void ScreenComponent::finish() {
		finished = true;
	}
}
