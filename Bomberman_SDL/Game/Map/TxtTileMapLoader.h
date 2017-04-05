#pragma once

#include <memory>
#include <string>
#include <vector>

namespace Bomberman {
	class TileMapBuilder;

	class TxtTileMapLoader {
	public:
		~TxtTileMapLoader();

		std::shared_ptr<TileMapBuilder> load(std::string fileName);

	private:
		std::shared_ptr<TileMapBuilder> _builder;

		bool processCommand(std::string command, std::vector<std::string> arguments);
	};
}