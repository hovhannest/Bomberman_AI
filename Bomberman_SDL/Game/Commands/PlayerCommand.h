#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../../Core/Command.h"

namespace Bomberman {
	class Player;
	class TileMap;

	class PlayerCommand : public Command {
	public:
		PlayerCommand(std::shared_ptr<Player> player, std::shared_ptr<TileMap> tileMap, std::string command, std::vector<std::string> arguments) {};

		void execute() {};

	private:
		void move() {};
		void setBomb() {};

		std::shared_ptr<TileMap> tileMap;
		std::shared_ptr<Player> player;
		std::string command;
		std::vector<std::string> arguments;
	};
}
