
#include "Screen.h"

#include <queue>
#include <SDL.h>
#include <SDL_events.h>

#include "SignalSender.h"
#include "ScreenManager.h"
#include "Log/LogLevel.h"
#include "Log/Log.h"
#include "Drawable.h"
#include "EventListener.h"
#include "Updatable.h"
#include "SignalHandler.h"

using namespace std;

namespace Bomberman {
	class SignalSenderImpl : public SignalSender {
	public:
		void sendSignal(Signal signal) {
			signalSent = true;
			this->signal = signal;
		}

		bool signalSent = false;
		Signal signal;
	};

	class ScreenManagerImpl : public ScreenManager {
	public:
		void addSignalHandler(shared_ptr<SignalHandler> signalHandler) {
			signalHandlers.push(signalHandler);
		}

		void addEventListener(shared_ptr<EventListener> eventListener) {
			eventListeners.push(eventListener);
		}

		void addDrawable(shared_ptr<Drawable> drawable) {
			drawables.push(drawable);
		}

		void addUpdatable(shared_ptr<Updatable> updatable) {
			updatables.push(updatable);
		}

		queue<shared_ptr<SignalHandler>> signalHandlers;
		queue<shared_ptr<EventListener>> eventListeners;
		queue<shared_ptr<Drawable>> drawables;
		queue<shared_ptr<Updatable>> updatables;
	};

	Screen::Screen(int width, int height, string name) : _name(name), _rectangle(0, 0, width, height), screenManager(new ScreenManagerImpl()), signalSender(new SignalSenderImpl()) {
		SDL_Window *w = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _rectangle.width, _rectangle.height, SDL_WINDOW_SHOWN);

		if (w == nullptr) {
			Log::get() << "Could not initialize window: \"" << name << "\": " << SDL_GetError() << "." << LogLevel::error;
			return;
		}

		window.reset(w, [](SDL_Window *w) {
			SDL_DestroyWindow(w);
		});

		SDL_Renderer *r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED);

		if (r == nullptr) {
			Log::get() << "Could not initialize renderer for window: \"" << name << "\": " << SDL_GetError() << "." << LogLevel::error;
			return;
		}

		_renderer.reset(r, [](SDL_Renderer *r) {
			SDL_DestroyRenderer(r);
		});

		// Enable transparencies
		if (SDL_SetRenderDrawBlendMode(_renderer.get(), SDL_BLENDMODE_BLEND)) {
			Log::get() << "Could not enable transparencies in window: \"" << name << "\": " << SDL_GetError() << "." << LogLevel::warning;
		}
	}

	Screen::~Screen() {

	}

	void Screen::sendSignal() {
		auto signalSender = dynamic_pointer_cast<SignalSenderImpl>(this->signalSender);
		if (!signalSender->signalSent) {
			return;
		}

		for (auto signalHandler : signalHandlers) {
			if (signalHandler->isEnabled()) {
				signalHandler->handleSignal(signalSender->signal);
			}
		}

		signalSender->signalSent = false;
	}

	void Screen::draw() {
		for (auto drawable : drawables) {
			if (drawable->isEnabled()) {
				drawable->draw();
			}
		}
	}

	void Screen::listenEvent(SDL_Event event) {
		for (auto eventListener : eventListeners) {
			if (eventListener->isEnabled()) {
				eventListener->listenEvent(event);
			}
		}
	}

	void Screen::update() {
		for (auto updatable : updatables) {
			if (updatable->isEnabled()) {
				updatable->update();
			}
		}
	}

	void Screen::postUpdate() {
		for (auto updatable : updatables) {
			if (updatable->isEnabled()) {
				updatable->postUpdate();
			}
		}
	}

	string Screen::name() const {
		return _name;
	}

	int Screen::width() const {
		return _rectangle.width;
	}

	int Screen::height() const {
		return _rectangle.height;
	}

	Rectangle Screen::rectangle() const {
		return _rectangle;
	}

	void Screen::setName(string name) {
		if (name != _name) {
			string prev = _name;
			_name = name;

			if (window) {
				SDL_SetWindowTitle(window.get(), name.c_str());
			}

			nameChanged(prev);
		}
	}

	void Screen::setWidth(int width) {
		if (width != _rectangle.width) {
			auto prev = _rectangle;
			_rectangle.width = width;

			refreshSize(prev);
		}
	}

	void Screen::setHeight(int height) {
		if (height != _rectangle.height) {
			auto prev = _rectangle;
			_rectangle.height = height;

			refreshSize(prev);
		}
	}

	void Screen::setSize(int width, int height) {
		if (width != _rectangle.width || height != _rectangle.height) {
			auto prev = _rectangle;
			_rectangle.width = width;
			_rectangle.height = height;

			refreshSize(prev);
		}
	}

	shared_ptr<SDL_Renderer> Screen::renderer() const {
		return _renderer;
	}

	shared_ptr<ScreenManager> Screen::getScreenManager() {
		return screenManager;
	}

	shared_ptr<SignalSender> Screen::getSignalSender() {
		return signalSender;
	}

	void Screen::refreshScreen() {
		signalHandlers.remove_if([](shared_ptr<SignalHandler> signalHandler) {
			return signalHandler->isFinished();
		});

		eventListeners.remove_if([](shared_ptr<EventListener> eventListener) {
			return eventListener->isFinished();
		});

		drawables.remove_if([](shared_ptr<Drawable> drawable) {
			return drawable->isFinished();
		});

		updatables.remove_if([](shared_ptr<Updatable> updatable) {
			return updatable->isFinished();
		});

		auto screenManager = dynamic_pointer_cast<ScreenManagerImpl>(this->screenManager);

		while (!screenManager->signalHandlers.empty()) {
			auto signalHandler = screenManager->signalHandlers.front();
			screenManager->signalHandlers.pop();

			signalHandlers.push_back(signalHandler);
		}

		while (!screenManager->eventListeners.empty()) {
			auto eventListener = screenManager->eventListeners.front();
			screenManager->eventListeners.pop();

			eventListeners.push_back(eventListener);
		}

		while (!screenManager->drawables.empty()) {
			auto drawable = screenManager->drawables.front();
			screenManager->drawables.pop();

			drawables.push_back(drawable);

			drawable->screenSizeChanged(Rectangle(), rectangle());
		}

		while (!screenManager->updatables.empty()) {
			auto updatable = screenManager->updatables.front();
			screenManager->updatables.pop();

			updatables.push_back(updatable);
		}
	}

	void Screen::nameChanged(string prevName) {

	}

	void Screen::sizeChanged(Rectangle previousSize) {
		for (auto drawable : drawables) {
			drawable->screenSizeChanged(previousSize, rectangle());
		}
	}

	void Screen::refreshSize(Rectangle previousSize) {
		if (window) {
			SDL_SetWindowSize(window.get(), _rectangle.width, _rectangle.height);
		}

		sizeChanged(previousSize);
	}
}
