#pragma once

#include "../../Core/Command.h"

#include <memory>

namespace Bomberman {
	class Console;
	class TileMap;

	class GameOverCommand : public Command {
	public:
		GameOverCommand(std::shared_ptr<Console> consoleLayer, std::shared_ptr<TileMap> tileMap) { }

		void execute();

	private:
		std::shared_ptr<Console> console;
		std::shared_ptr<TileMap> tileMap;
	};
}
