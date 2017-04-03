#pragma once

union SDL_Event;

#include "ScreenComponent.h"

namespace Bomberman {
	class EventListener : public ScreenComponent {
	public:
		virtual void listenEvent(SDL_Event event) = 0;
	};
}