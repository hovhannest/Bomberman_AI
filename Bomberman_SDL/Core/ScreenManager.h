#pragma once

#include <memory>
#include <queue>

namespace Bomberman {
	class EventListener;
	class Drawable;
	class Updatable;
	class SignalHandler;

	class ScreenManager {
	public:
		virtual ~ScreenManager() {};

		virtual void addSignalHandler(std::shared_ptr<SignalHandler> signalHandler) = 0;
		virtual void addEventListener(std::shared_ptr<EventListener> eventListener) = 0;
		virtual void addDrawable(std::shared_ptr<Drawable> drawable) = 0;
		virtual void addUpdatable(std::shared_ptr<Updatable> updatable) = 0;
	};
}