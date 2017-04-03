
#include "CommandFactory.h"
#include "..\Core\Utils\Exception.h"
#include "..\Core\Utils\StringUtils.h"
#include "..\Core\LoopQuiter.h"
#include "Constants.h"
#include "Commands\QuitCommand.h"
#include "Commands\GameOverCommand.h"
#include "Commands\PlayerCommand.h"
#include "..\Core\Utils\Pointer.h"


using namespace std;
using namespace Bomberman::Constants;

namespace Bomberman {
	template <typename T>
	bool _lock(weak_ptr<T> in, shared_ptr<T>& out, string component) {
		bool result = lockWeakPointer(in, out);

		if (!result) {
			Log::get() << "No " << component << " for CommandFactory" << LogLevel::error;
		}

		return result;
	}

	CommandFactory::~CommandFactory() {

	}

	void CommandFactory::setPlayer(weak_ptr<Player> player) {
		this->player = player;
	}

	void CommandFactory::setTileMap(weak_ptr<TileMap> tileMap) {
		this->tileMap = tileMap;
	}

	void CommandFactory::setConsole(weak_ptr<Console> console) {
		this->console = console;
	}

	void CommandFactory::setLoopQuiter(weak_ptr<LoopQuiter> loopQuiter) {
		this->loopQuiter = loopQuiter;
	}

	shared_ptr<Command> CommandFactory::call(string function, vector<string> arguments) {
		shared_ptr<Command> result;

		function = StringUtils::toLower(function);
		for (int n = 0; n < int(arguments.size()); ++n) {
			arguments[n] = StringUtils::toLower(arguments[n]);
		}

		if (FUNC_EXIT == function) {
			shared_ptr<LoopQuiter> loopQuiter;
			if (_lock(this->loopQuiter, loopQuiter, "LoopQuiter")) {
				result.reset(new QuitCommand(loopQuiter));
			}
		}
		else if (FUNC_GAME_OVER == function) {
			shared_ptr<Console> console;
			shared_ptr<TileMap> tileMap;
			if (_lock(this->console, console, "Console") &&
				_lock(this->tileMap, tileMap, "TileMap")) {
				result.reset(new GameOverCommand(console, tileMap));
			}
		}
		else {
			throw InvalidFunctionException();
		}

		return result;
	}

	shared_ptr<Command> CommandFactory::sendMessage(string receiver, string message, vector<string> arguments) {
		shared_ptr<Command> result;

		receiver = StringUtils::toLower(receiver);
		message = StringUtils::toLower(message);
		for (int n = 0; n < int(arguments.size()); ++n) {
			arguments[n] = StringUtils::toLower(arguments[n]);
		}

		if (receiver == OBJ_PLAYER) {
			shared_ptr<Player> player;
			shared_ptr<TileMap> tileMap;
			if (_lock(this->player, player, "Player") &&
				_lock(this->tileMap, tileMap, "TileMap")) {
				result.reset(new PlayerCommand(player, tileMap, message, arguments));
			}
		}
		else {
			throw InvalidReceiverException();
		}

		return result;
	}
}
