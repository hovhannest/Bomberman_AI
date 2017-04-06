
#include "XmlTileMapLoader.h"

#include <memory>
#include <string>
#include "DummyTileMapBuilder.h"
#include "../Elements/Item.h"
#include "../../Core/Utils/OperatingSystem.h"
#include "../../Core/Log/LogLevel.h"
#include "../../Core/Log/Log.h"
#include "../../Core/Utils/Exception.h"

using namespace std;
using namespace tinyxml2;

namespace Bomberman {
	XmlTileMapLoader::~XmlTileMapLoader() {

	}

	std::shared_ptr<TileMapBuilder> XmlTileMapLoader::load(std::string fileName) {
 		reset();

		this->fileName = fileName;
		string fullPath = getPath({ "maps" }, fileName);

		if (document.LoadFile(fullPath.c_str()) == XML_SUCCESS) {
			if (document.NoChildren()) {
				Log::get() << "Empty map file \"" << fileName << "\"." << LogLevel::error;
				_error = true;
			}
			else {
				loadDimension(document.RootElement());
				loadName(document.RootElement()->FirstChildElement("name"));
				loadPlayer(document.RootElement()->FirstChildElement("player"));
				loadBricks(document.RootElement()->FirstChildElement("bricks"));
			}
		}
		else {
			_error = true;
		}

		if (_error) {
			Log::get() << "Error loading map \"" << fileName << "\"." << OpeningFileErrorException();
			builder.reset();
		}

		return builder;
	}

	void XmlTileMapLoader::reset() {
		fileName.clear();
		document.Clear();
		_error = false;
		builder.reset(new DummyTileMapBuilder);
	}

 	void XmlTileMapLoader::loadDimension(tinyxml2::XMLElement *root) {
		int width = 0;
		int height = 0;

		if (root->QueryIntAttribute("width", &width) != XML_SUCCESS) {
			_error = true;
			Log::get() << "Invalid width in map file " << fileName << "." << LogLevel::error;
		}

		if (root->QueryIntAttribute("height", &height) != XML_SUCCESS) {
			_error = true;
			Log::get() << "Invalid height in map file " << fileName << "." << LogLevel::error;
		}

		dynamic_pointer_cast<DummyTileMapBuilder>(builder)->_width = width;
		dynamic_pointer_cast<DummyTileMapBuilder>(builder)->_height = height;
	}

	void XmlTileMapLoader::loadName(XMLElement *nameNode) {
		if (nameNode == nullptr) {
			Log::get() << "No name in map file \"" << fileName << "\"." << LogLevel::error;
			_error = true;
		}

		dynamic_pointer_cast<DummyTileMapBuilder>(builder)->_name = nameNode->GetText();
	}

	void XmlTileMapLoader::loadPlayer(XMLElement *playerNode) {
		shared_ptr<Player> player(new Player());

		if (playerNode == nullptr) {
			Log::get() << "No information for player in map file \"" << fileName << "\"." << LogLevel::error;
			_error = true;
			return;
		}

		auto position = playerNode->FirstChildElement("position");
		if (position == nullptr) {
			Log::get() << "No position for player in map file \"" << fileName << "\"." << LogLevel::error;
			_error = true;
			return;
		}

		if (position->QueryIntAttribute("column", &player->position().i) != XML_SUCCESS) {
			Log::get() << "Invalid column for player in map file \"" << fileName << "\"." << LogLevel::error;
			_error = true;
		}

		if (position->QueryIntAttribute("row", &player->position().j) != XML_SUCCESS) {
			Log::get() << "Invalid row for player in map file \"" << fileName << "\"." << LogLevel::error;
			_error = true;
		}

		dynamic_pointer_cast<DummyTileMapBuilder>(builder)->_player = player;
	}

	void XmlTileMapLoader::loadBricks(XMLElement *bricks) {
		if (bricks == nullptr) {
			Log::get() << "No bricks node in map file " << fileName << "." << LogLevel::warning;
			return;
		}

		int bricksCount = 1;
		auto brick = bricks->FirstChildElement("brick");
		bool error;
		while (brick) {
			error = false;

			Coordinate c;
			if (brick->QueryIntAttribute("column", &c.i) != XML_SUCCESS) {
				Log::get() << "Brick number " << bricksCount << " has invalid column in map file \"" << fileName << "\"." << LogLevel::error;
				error = true;
			}

			if (brick->QueryIntAttribute("row", &c.j) != XML_SUCCESS) {
				Log::get() << "Brick number " << bricksCount << " has invalid row in map file \"" << fileName << "\"." << LogLevel::error;
				error = true;
			}

			bool destructible = false;
			auto result = brick->QueryBoolAttribute("destructible", &destructible);
			if (result != XML_SUCCESS  && result != XML_NO_ATTRIBUTE) {
				Log::get() << "Brick number " << bricksCount << " has invalid \"destructible\" value in map file \"" << fileName << "\", using default value." << LogLevel::warning;
			}

			if (!error) {
				dynamic_pointer_cast<DummyTileMapBuilder>(builder)->_bricks.push_back(Brick(c, destructible));
			}
			else {
				_error |= error;
			}

			brick = brick->NextSiblingElement();
			++bricksCount;
		}
	}
}
