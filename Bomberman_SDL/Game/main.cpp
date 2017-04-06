
#include <iostream>
#include <memory>

#include "../Core/Engine.h"
#include "Configuration.h"
#include "../Core/MainLoop.h"
#include "../Core/Screen.h"
#include "CommandFactory.h"
#include "MapSetter.h"
#include "InGameCommandQueue.h"
#include "EventListeners/ConsoleEvents.h"
#include "EventListeners/PlayerEvents.h"
#include "Layers/ConsoleLayer.h"
#include "Layers/GameLayer.h"
#include "Layers/HudLayer.h"
#include "../Core/ScreenManager.h"
#include "../Core/Log/Log.h"
#include "Console.h"

using namespace Bomberman;
using namespace std;

int main(int argc, char* argv[])
{
	try
	{
		Engine engine;
		Configuration config("config.xml");
		MainLoop loop;
		std::shared_ptr<Screen> screen(new Screen(config.viewportWidth(), config.viewportHeight(), config.viewportTitle()));
		auto commandFactory = make_shared<CommandFactory>();
		auto console = make_shared<Console>(commandFactory);
		auto mapSetter = make_shared<MapSetter>();

		// Screen components
		auto commandQueue = make_shared<InGameCommandQueue>();
		auto consoleEvents = make_shared<ConsoleEvents>();
		auto playerEvents = make_shared<PlayerEvents>();
		auto consoleLayer = make_shared<ConsoleLayer>();
		auto gameLayer = make_shared<GameLayer>();
		auto hudLayer = make_shared<HudLayer>();

		// Initialize all the screen components
		consoleEvents->setConsole(console);
		playerEvents->setCommandFactory(commandFactory);
		playerEvents->setCommandQueue(commandQueue);
		gameLayer->setSignalSender(screen->getSignalSender());

		// Load everything
		consoleLayer->load(screen->renderer());
		gameLayer->load(screen->renderer());
		hudLayer->load(screen->renderer());

		// Register everything
		auto screenManager = screen->getScreenManager();

		screenManager->addUpdatable(commandQueue);
		screenManager->addSignalHandler(commandQueue);

		screenManager->addEventListener(consoleEvents);
		screenManager->addSignalHandler(consoleEvents);

		screenManager->addEventListener(playerEvents);
		screenManager->addSignalHandler(playerEvents);

		screenManager->addEventListener(gameLayer);
		screenManager->addDrawable(gameLayer);
		screenManager->addUpdatable(gameLayer);
		screenManager->addSignalHandler(gameLayer);

		screenManager->addDrawable(hudLayer);
		screenManager->addSignalHandler(hudLayer);

		screenManager->addDrawable(consoleLayer);
		screenManager->addUpdatable(consoleLayer);
		screenManager->addSignalHandler(consoleLayer);
		Log::get().addLogger(consoleLayer);

		// Register other stuff
		commandFactory->setConsole(console);
		commandFactory->setLoopQuiter(loop.quiter());
		console->setCommandQueue(commandQueue);
		console->setConsoleLayer(consoleLayer);
		console->setSignalSender(screen->getSignalSender());
		mapSetter->setCommandFactory(commandFactory);
		mapSetter->setGameLayer(gameLayer);
		mapSetter->setHudLayer(hudLayer);

		mapSetter->setMap("map1.txt");
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
