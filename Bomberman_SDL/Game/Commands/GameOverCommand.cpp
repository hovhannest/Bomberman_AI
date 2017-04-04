
#include "GameOverCommand.h"
#include "../Console.h"
#include "../Map/TileMap.hpp"

using namespace std;

namespace Bomberman 
{
	void GameOverCommand::execute() 
	{
		if (console->visible()) {
			console->hide();
		}

		tileMap->endGame();

		_finished = true;
	}
}
