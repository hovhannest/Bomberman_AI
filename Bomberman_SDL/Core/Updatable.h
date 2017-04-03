#pragma once

#include "ScreenComponent.h"

namespace Bomberman {
	class Updatable : public ScreenComponent {
	public:
		virtual void update() = 0;

		virtual void postUpdate() {};
	};
}
