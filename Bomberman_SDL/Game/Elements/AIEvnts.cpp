
#include "AIEvents.h"

#include <SDL_events.h>
#include "..\Signal.h"
#include "..\Constants.h"
#include <memory>
#include "..\..\Core\Command.h"
#include "..\CommandFactory.h"
#include "..\..\Core\CommandQueue.h"
#include "..\..\Core\Utils\Pointer.h"
#include "..\..\Core\Log\Log.h"
#include "..\..\Core\Log\LogLevel.h"

#include "SWI-cpp.h"
#include "SWI-Prolog.h"

using namespace std;
using namespace Bomberman::Constants;

namespace Bomberman {

	template <typename T>
	bool _lock(weak_ptr<T> in, shared_ptr<T>& out, string component) {
		bool result = lockWeakPointer(in, out);

		if (!result) {
			Log::get() << "No " << component << " for PlayerEvents" << LogLevel::error;
		}

		return result;
	}

	AIEvents::AIEvents()
	{
		predicate_t hello;
		term_t t0 = NULL;

		long n = 5;
		int rval;

		PL_put_integer(t0, n);

		hello = PL_predicate("hello", 1, NULL);

		rval = PL_call_predicate(NULL, PL_Q_NORMAL, hello, t0);
	}

	void AIEvents::listenEvent(SDL_Event event)
	{
		shared_ptr<Command> command;
		shared_ptr<CommandFactory> commandFactory;
		shared_ptr<CommandQueue> commandQueue;

		if (!_lock(this->commandFactory, commandFactory, "CommandFactory") ||
			!_lock(this->commandQueue, commandQueue, "CommandQueue")) {
			return;
		}


		if (command) {
			commandQueue->addCommand(command);
		}
	}


	void AIEvents::handleSignal(Signal signal)
	{
		if (signalIn(signal, { Signal::InGame, Signal::HideConsole, Signal::PlayerAlive })) {
			EventListener::enable();
		}
		else {
			EventListener::disable();
		}
	}

	void AIEvents::setCommandFactory(std::weak_ptr<CommandFactory> commandFactory)
	{
		this->commandFactory = commandFactory;
	}


	void AIEvents::setCommandQueue(std::weak_ptr<CommandQueue> commandQueue)
	{
		this->commandQueue = commandQueue;
	}

}