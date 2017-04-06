
#include "GameOverCommand.h"
#include "../Console.h"
#include "../Map/TileMap.h"

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
