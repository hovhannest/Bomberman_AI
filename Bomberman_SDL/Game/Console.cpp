
#include "Console.h"
#include "../Core/Log/Log.h"
#include "../Core/Log/LogLevel.h"
#include "../Core/SignalSender.h"
#include <SDL.h>
#include "Signal.h"
#include "Layers/ConsoleLayer.h"
#include "../Core/CommandQueue.h"
#include "../Core/Utils/Pointer.h"

using namespace std;

namespace Bomberman {
	const int Console::BUFFER_SIZE = 80;

	template <typename T>
	bool _lock(weak_ptr<T> in, shared_ptr<T>& out, string componentName) {
		bool result = lockWeakPointer(in, out);

		if (!result) {
			Log::get() << "No " << componentName << " for Console" << LogLevel::error;
		}

		return result;
	}

	Console::Console(shared_ptr<CommandFactory> commandFactory) : parser(commandFactory), _visible(false) {

	}

	bool Console::visible() const {
		return _visible;
	}

	void Console::hide() {
		shared_ptr<ConsoleLayer> consoleLayer;
		shared_ptr<SignalSender> signalSender;
		if (!_lock(this->consoleLayer, consoleLayer, "ConsoleLayer") ||
			!_lock(this->signalSender, signalSender, "SignalSender")) {
			return;
		}

		signalSender->sendSignal(Signal::HideConsole);

		_visible = false;

		SDL_StopTextInput();
	}

	void Console::show() {
		shared_ptr<ConsoleLayer> consoleLayer;
		shared_ptr<SignalSender> signalSender;
		if (!_lock(this->consoleLayer, consoleLayer, "ConsoleLayer") ||
			!_lock(this->signalSender, signalSender, "SignalSender")) {
			return;
		}

		signalSender->sendSignal(Signal::ShowConsole);

		_visible = true;

		SDL_StartTextInput();
	}

	void Console::addToBuffer(char character) {
		shared_ptr<ConsoleLayer> consoleLayer;
		if (!_lock(this->consoleLayer, consoleLayer, "ConsoleLayer")) {
			return;
		}

		buffer += character;

		consoleLayer->setInput(buffer);
	}

	void Console::addToBuffer(string str) {
		shared_ptr<ConsoleLayer> consoleLayer;
		if (!_lock(this->consoleLayer, consoleLayer, "ConsoleLayer")) {
			return;
		}

		if (buffer.size() < BUFFER_SIZE) {
			buffer += str;

			consoleLayer->setInput(buffer);
		}
	}

	void Console::removeLastFromBuffer() {
		shared_ptr<ConsoleLayer> consoleLayer;
		if (!_lock(this->consoleLayer, consoleLayer, "ConsoleLayer")) {
			return;
		}

		if (!buffer.empty()) {
			buffer.pop_back();

			if (!buffer.empty()) {
				consoleLayer->setInput(buffer);
			}
			else {
				consoleLayer->clearInput();
			}
		}
	}

	void Console::clearBuffer() {
		shared_ptr<ConsoleLayer> consoleLayer;
		if (!_lock(this->consoleLayer, consoleLayer, "ConsoleLayer")) {
			return;
		}

		buffer.clear();

		consoleLayer->clearInput();
	}

	void Console::commitBuffer() {
		shared_ptr<CommandQueue> commandQueue;
		if (!_lock(this->commandQueue, commandQueue, "CommandQueue")) {
			return;
		}

		if (buffer.empty()) {
			return;
		}

		Log::get() << buffer << LogLevel::info;
		auto cmds = parser.parse(buffer);

		while (!cmds.empty()) {
			auto cmd = cmds.front();
			commandQueue->addCommand(cmd);
			cmds.pop();
		}

		clearBuffer();
	}

	void Console::setCommandQueue(weak_ptr<CommandQueue> commandQueue) {
		this->commandQueue = commandQueue;
	}

	void Console::setConsoleLayer(weak_ptr<ConsoleLayer> consoleLayer) {
		this->consoleLayer = consoleLayer;

		if (!consoleLayer.expired()) {
			_visible = consoleLayer.lock()->Drawable::isEnabled();
		}
	}

	void Console::setSignalSender(weak_ptr<SignalSender> signalSender) {
		this->signalSender = signalSender;
	}
}
