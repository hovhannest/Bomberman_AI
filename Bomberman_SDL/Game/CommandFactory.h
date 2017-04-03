#pragma once

#include <memory>
#include <string>
#include <vector>

namespace Bomberman {
	class Command;
	class Console;
	class LoopQuiter;
	class Player;
	class TileMap;

	class CommandFactory {
	public:
		~CommandFactory();

		void setPlayer(std::weak_ptr<Player> player);

		void setTileMap(std::weak_ptr<TileMap> tileMap);

		void setLoopQuiter(std::weak_ptr<LoopQuiter> loopQuiter);

		void setConsole(std::weak_ptr<Console> console);

		std::shared_ptr<Command> call(std::string function, std::vector<std::string> arguments);

		std::shared_ptr<Command> sendMessage(std::string receiver, std::string message, std::vector<std::string> arguments = {});

	private:
		std::weak_ptr<Player> player;
		std::weak_ptr<TileMap> tileMap;
		std::weak_ptr<LoopQuiter> loopQuiter;
		std::weak_ptr<Console> console;
	};
}