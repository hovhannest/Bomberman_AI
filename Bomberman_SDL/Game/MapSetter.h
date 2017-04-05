#pragma once

#include <memory>
#include <string>

namespace Bomberman {
	class CommandFactory;
	class GameLayer;
	class HudLayer;

	class MapSetter {
	public:
		bool setMap(std::string fileName);

		void setCommandFactory(std::weak_ptr<CommandFactory> commandFactory);
		void setGameLayer(std::weak_ptr<GameLayer> gameLayer);
		void setHudLayer(std::weak_ptr<HudLayer> hudLayer);

	private:
		std::weak_ptr<CommandFactory> commandFactory;
		std::weak_ptr<GameLayer> gameLayer;
		std::weak_ptr<HudLayer> hudLayer;
	};
}