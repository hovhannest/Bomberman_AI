
#include <iostream>
#include <memory>

#include "../Core/Engine.h"
#include "Configuration.h"
#include "../Core/MainLoop.h"
#include "../Core/Screen.h"

using namespace Bomberman;

int main(int argc, char* argv[])
{
	try
	{
		Engine engine;
		Configuration config("config.xml");
		MainLoop loop;
		std::shared_ptr<Screen> screen(new Screen(config.viewportWidth(), config.viewportHeight(), config.viewportTitle()));

		//Game
		loop.addScreen(screen);
		loop.run();
	}
	catch (const std::exception&)
	{
		return -1;
	}

	return 0;
}
