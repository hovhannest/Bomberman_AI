#pragma once


#include <memory>
#include <string>
#include <list>

#include "Math\Rectangle.h"

union SDL_Event;
struct SDL_Renderer;
struct SDL_Window;

namespace Bomberman {
	class EventListener;
	class Drawable;
	class ScreenManager;
	class Updatable;
	class SignalHandler;
	class SignalSender;

	class Screen {
	public:
		Screen(int width, int height, std::string name);
		~Screen();

		void sendSignal();
		void listenEvent(SDL_Event event);
		void draw();
		void update();
		void postUpdate();

		std::string name() const;
		int width() const;
		int height() const;
		Rectangle rectangle() const;

		void setName(std::string name);
		void setWidth(int width);
		void setHeight(int height);
		void setSize(int width, int height);

		std::shared_ptr<SDL_Renderer> renderer() const;

		std::shared_ptr<ScreenManager> getScreenManager();
		std::shared_ptr<SignalSender> getSignalSender();

		void refreshScreen();

	protected:
		virtual void nameChanged(std::string prevName);
		virtual void sizeChanged(Rectangle previousSize);

	private:
		void refreshSize(Rectangle previousSize);

		std::string _name;
		std::shared_ptr<SDL_Window> window;
		std::shared_ptr<SDL_Renderer> _renderer;
		Rectangle _rectangle;

		std::shared_ptr<ScreenManager> screenManager;
		std::shared_ptr<SignalSender> signalSender;

		std::list<std::shared_ptr<SignalHandler>> signalHandlers;
		std::list<std::shared_ptr<EventListener>> eventListeners;
		std::list<std::shared_ptr<Drawable>> drawables;
		std::list<std::shared_ptr<Updatable>> updatables;
	};
}