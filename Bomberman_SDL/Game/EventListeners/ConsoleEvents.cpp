
#include "ConsoleEvents.h"

#include <SDL.h>
#include "../../Core/Utils/Pointer.h"
#include "../../Core/Log/Log.h"
#include "../../Core/Log/LogLevel.h"
#include "../Signal.h"
#include "../Console.h"

using namespace std;

namespace Bomberman {
	void ConsoleEvents::listenEvent(SDL_Event event) {
		shared_ptr<Console> console;
		if (!lockWeakPointer(this->console, console)) {
			Log::get() << "No Console for ConsoleEvents" << LogLevel::error;
			return;
		}

		if (console->visible()) {
			if (SDL_KEYUP == event.type) {
				auto keySym = event.key.keysym.sym;

				if (SDLK_ESCAPE == keySym) {
					console->hide();
				}
				else if (SDLK_RETURN == keySym) {
					console->commitBuffer();
				}
			}
			else if (SDL_KEYDOWN == event.type) {
				auto keySym = event.key.keysym.sym;

				if (SDLK_BACKSPACE == keySym) {
					console->removeLastFromBuffer();
				}
			}
			else if (SDL_TEXTINPUT == event.type) {
				string inputText = event.text.text;
				console->addToBuffer(inputText);
			}
		}
		else if (event.type == SDL_KEYUP && SDLK_BACKSPACE == event.key.keysym.sym) {
			console->show();
		}
	}

	void ConsoleEvents::setConsole(weak_ptr<Console> console) {
		this->console = console;
	}

	void ConsoleEvents::handleSignal(Signal signal) {
		if (signalIn(signal, { Signal::InGame, Signal::ShowConsole, Signal::HideConsole, Signal::PlayerAlive, Signal::PlayerDead })) {
			EventListener::enable();
		}
		else {
			EventListener::disable();
		}
	}
}