
#include "MapSetter.h"

#include "Map/TileMap.h"
#include "../Core/Utils/OperatingSystem.h"
#include "CommandFactory.h"
#include "Map/TxtTileMapLoader.h"
#include "../Core/Log/Log.h"
#include "../Core/Log/LogLevel.h"
#include "../Core/Utils/Pointer.h"
#include "Layers/GameLayer.h"
#include "Layers/HudLayer.h"

using namespace std;

namespace Bomberman {
	template <typename T>
	bool _lock(weak_ptr<T> in, shared_ptr<T>& out, string component) {
		bool result = lockWeakPointer(in, out);

		if (!result) {
			Log::get() << "No " << component << " for MapSetter" << LogLevel::error;
		}

		return result;
	}

	shared_ptr<TileMap> loadMap(string fileName) {
		shared_ptr<TileMap> result;
		string mapPath = getPath({ "maps" }, fileName);

		auto mapLoader = TxtTileMapLoader().load(fileName);

		if (mapLoader) {
			result = make_shared<TileMap>(mapLoader);
		}

		return result;
	}

	bool MapSetter::setMap(string fileName) {
		shared_ptr<CommandFactory> commandFactory;
		shared_ptr<GameLayer> gameLayer;
		shared_ptr<HudLayer> hudLayer;
		if (!_lock(this->commandFactory, commandFactory, "CommandFactory") ||
			!_lock(this->gameLayer, gameLayer, "GameLayer") ||
			!_lock(this->hudLayer, hudLayer, "HudLayer")) {
			return false;
		}

		bool result = false;
		shared_ptr<TileMap> tileMap = loadMap(fileName);

		if (nullptr != tileMap) {
			commandFactory->setTileMap(tileMap);
			commandFactory->setPlayer(tileMap->player());

			gameLayer->setTileMap(tileMap);

			hudLayer->setTileMap(tileMap);
			hudLayer->setPlayer(tileMap->player());

			result = true;
		}
		else {
			Log::get() << "Could not open map \"" << fileName << "\"" << LogLevel::error;
		}


		return result;
	}

	void MapSetter::setCommandFactory(weak_ptr<CommandFactory> commandFactory) {
		this->commandFactory = commandFactory;
	}

	void MapSetter::setGameLayer(weak_ptr<GameLayer> gameLayer) {
		this->gameLayer = gameLayer;
	}

	void MapSetter::setHudLayer(weak_ptr<HudLayer> hudLayer) {
		this->hudLayer = hudLayer;
	}
}