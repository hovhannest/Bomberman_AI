
#include "TxtTileMapLoader.h"

#include <cstdlib>
#include <fstream>
#include <sstream>

#include "../Constants.h"
#include "../../Core/Utils/StringUtils.h"
#include "../../Core/Log/LogLevel.h"
#include "../../Core/Log/Log.h"
#include "../../Core/Math/Coordinate.h"
#include "../../Core/Utils/OperatingSystem.h"
#include "DummyTileMapBuilder.h"
#include "../../Core/Utils/Exception.h"
#include "../Elements/Enemy.h"
#include "../Elements/Player.h"
#include "../Elements/Brick.h"
#include "../Elements/Item.h"
#include "../../AI/PrologHelper.h"

using namespace std;
using namespace Bomberman::Constants;

namespace Bomberman {
	class CommandReader {
	public:
		CommandReader(string str) : _validCommand(false) {
			vector<string> tokens = StringUtils::split(str, ' ');

			if (tokens.empty()) {
				_validCommand = false;
				return;
			}

			_command = tokens[0];

			for (int n = 1; n < tokens.size(); ++n) {
				_arguments.push_back(tokens[n]);
			}

			verifyCommand();
		}

		bool validCommand() {
			return _validCommand;
		}

		string getCommand() {
			return _command;
		}

		vector<string> getArguments() {
			return _arguments;
		}

	private:
		bool _validCommand;
		string _command;
		vector<string> _arguments;

		void verifyCommand() {
			if (_command == MAP_CMD_NAME) {
				_validCommand = !_arguments.empty();
			}
			else if (_command == MAP_CMD_ENEMY_RANGE) {
				_validCommand = _arguments.size() == 2;
			}
			else if (_command == MAP_CMD_ENEMY_SPEED) {
				_validCommand = _arguments.size() == 2;
			}
			else if (_command == MAP_CMD_ENEMY) {
				_validCommand = _arguments.size() == 3;
			}
			else if (_command == MAP_CMD_PLAYER) {
				_validCommand = _arguments.size() == 2;
			}
			else if (_command == MAP_CMD_SIZE) {
				_validCommand = _arguments.size() == 2;
			}
			else if (_command == MAP_CMD_SINGLE) {
				_validCommand = _arguments.size() == 3;
			}
			else if (_command == MAP_CMD_BLOCK) {
				_validCommand = _arguments.size() == 5;
			}
			else if (_command == MAP_CMD_VLINE) {
				_validCommand = _arguments.size() == 4;
			}
			else if (_command == MAP_CMD_HLINE) {
				_validCommand = _arguments.size() == 4;
			}
			else if (_command == MAP_CMD_ITEM) {
				_validCommand = _arguments.size() == 3;
			}
			else if (_command == MAP_CMD_DOOR) {
				_validCommand = _arguments.size() == 2;
			}
			else {
				_validCommand = false;
			}
		}
	};

	void printError(string line) {
		Log::get() << "Invalid command in map file: " << line << LogLevel::error;
	}

	bool buildCoordinate(string i, string j, Coordinate& coordinate) {
		try {
			coordinate.i = stoi(i);
			coordinate.j = stoi(j);
		}
		catch (logic_error&) {
			return false;
		}

		return true;
	}

	TxtTileMapLoader::~TxtTileMapLoader() {

	}

	shared_ptr<TileMapBuilder> TxtTileMapLoader::load(string fileName) {
		ifstream file(getPath({ "maps" }, fileName));

		if (file) {
			Log::get() << "Reading map file: " << fileName << LogLevel::info;
			_builder.reset(new DummyTileMapBuilder());

			string line;
			while (getline(file, line)) {
				line = StringUtils::trim(line);

				if (line.empty() || line[0] == '#') {
					continue;
				}

				CommandReader commandReader(line);
				string command = commandReader.getCommand();
				vector<string> arguments = commandReader.getArguments();

				if (!(commandReader.validCommand() && processCommand(command, arguments))) {
					printError(line);
					continue;
				}
			}
		}
		else {
			Log::get() << "Could not open file: " << fileName << OpeningFileErrorException();
			_builder.reset();
		}

		return _builder;
	}

	bool TxtTileMapLoader::processCommand(string command, vector<string> arguments) {
		auto builder = dynamic_pointer_cast<DummyTileMapBuilder>(_builder);

		if (command == MAP_CMD_NAME) {
			builder->_name = StringUtils::join(arguments, ' ');
		}
		else if (command == MAP_CMD_ENEMY_RANGE) {
			int range;

			if (!StringUtils::tryParseInt(arguments[1], range)) {
				return false;
			}

			if (arguments[0] == ENEMY_EASY) {
				builder->enemiesRange.easy = range;
			}
			else if (arguments[0] == ENEMY_MEDIUM) {
				builder->enemiesRange.medium = range;
			}
			else if (arguments[0] == ENEMY_HARD) {
				builder->enemiesRange.hard = range;
			}
			else {
				return false;
			}
		}
		else if (command == MAP_CMD_ENEMY_SPEED) {
			int speed;

			if (!StringUtils::tryParseInt(arguments[1], speed)) {
				return false;
			}

			if (arguments[0] == ENEMY_EASY) {
				builder->enemiesSpeed.easy = speed;
			}
			else if (arguments[0] == ENEMY_MEDIUM) {
				builder->enemiesSpeed.medium = speed;
			}
			else if (arguments[0] == ENEMY_HARD) {
				builder->enemiesSpeed.hard = speed;
			}
			else {
				return false;
			}
		}
		else if (command == MAP_CMD_ENEMY) {
			if (arguments[0] != ENEMY_EASY && arguments[0] != ENEMY_MEDIUM && arguments[0] != ENEMY_HARD) {
				return false;
			}

			Coordinate position;
			if (!buildCoordinate(arguments[1], arguments[2], position)) {
				return false;
			}

			Enemy enemy(arguments[0], position);

			builder->_enemies.push_back(enemy);
		}
		else if (command == MAP_CMD_PLAYER) {
			shared_ptr<Player> player(new Player());

			if (!buildCoordinate(arguments[0], arguments[1], player->position())) {
				return false;
			}

			builder->_player = player;
			PrologHelper::GetInstance()->AgentLocation(player->position().i, player->position().j);
		}
		else if (command == MAP_CMD_SIZE) {
			Coordinate coordinate;

			if (!buildCoordinate(arguments[0], arguments[1], coordinate)) {
				return false;
			}

			builder->_width = coordinate.i;
			builder->_height = coordinate.j;
			
			PrologHelper::GetInstance()->Init(coordinate.i, coordinate.j);
		}
		else if (command == MAP_CMD_SINGLE) {
			Coordinate coordinate;

			if (!buildCoordinate(arguments[1], arguments[2], coordinate)) {
				return false;
			}

			if (arguments[0] == MAP_OBJ_COMMON_BRICK) {
				builder->_bricks.push_back(Brick(coordinate));
				PrologHelper::GetInstance()->AddUndestructableWall(coordinate.i, coordinate.j);
			}
			else if (arguments[0] == MAP_OBJ_DESTRUCT_BRICK) {
				builder->_bricks.push_back(Brick(coordinate, true));
				PrologHelper::GetInstance()->AddDestructableWall(coordinate.i, coordinate.j);
			}
			else {
				return false;
			}
		}
		else if (command == MAP_CMD_BLOCK) {
			Coordinate start, end;

			if (!buildCoordinate(arguments[1], arguments[2], start)) {
				return false;
			}
			if (!buildCoordinate(arguments[3], arguments[4], end)) {
				return false;
			}

			if (start.i > end.i) {
				swap(start.i, end.i);
			}

			if (start.j > end.j) {
				swap(start.j, end.j);
			}

			bool destructible;
			if (arguments[0] == MAP_OBJ_COMMON_BRICK) {
				destructible = false;
			}
			else if (arguments[0] == MAP_OBJ_DESTRUCT_BRICK) {
				destructible = true;
			}
			else {
				return false;
			}

			Coordinate pos;
			for (pos.i = start.i; pos.i <= end.i; ++pos.i) {
				for (pos.j = start.j; pos.j <= end.j; ++pos.j) {
					builder->_bricks.push_back(Brick(pos, destructible));
					if (destructible)
					{
						PrologHelper::GetInstance()->AddDestructableWall(pos.i, pos.j);
					}
					else
					{
						PrologHelper::GetInstance()->AddUndestructableWall(pos.i, pos.j);
					}
				}
			}
		}
		else if (command == MAP_CMD_HLINE) {
			Coordinate start;
			int length;

			if (!buildCoordinate(arguments[1], arguments[2], start)) {
				return false;
			}

			if (!StringUtils::tryParseInt(arguments[3], length)) {
				return false;
			}

			bool destructible;
			if (arguments[0] == MAP_OBJ_COMMON_BRICK) {
				destructible = false;
			}
			else if (arguments[0] == MAP_OBJ_DESTRUCT_BRICK) {
				destructible = true;
			}
			else {
				return false;
			}

			Coordinate pos = start;
			for (int n = 0; n < abs(length); ++n) {
				builder->_bricks.push_back(Brick(pos, destructible));
				if (length < 0) {
					--pos.i;
				}
				else {
					++pos.i;
				}
			}
		}
		else if (command == MAP_CMD_VLINE) {
			Coordinate start;
			int length;

			if (!buildCoordinate(arguments[1], arguments[2], start)) {
				return false;
			}

			if (!StringUtils::tryParseInt(arguments[3], length)) {
				return false;
			}

			bool destructible;
			if (arguments[0] == MAP_OBJ_COMMON_BRICK) {
				destructible = false;
			}
			else if (arguments[0] == MAP_OBJ_DESTRUCT_BRICK) {
				destructible = true;
			}
			else {
				return false;
			}

			Coordinate pos = start;
			for (int n = 0; n < abs(length); ++n) {
				builder->_bricks.push_back(Brick(pos, destructible));
				if (length < 0) {
					--pos.j;
				}
				else {
					++pos.j;
				}
			}
		}
		else if (command == MAP_CMD_ITEM) {
			Coordinate pos;
			if (!buildCoordinate(arguments[1], arguments[2], pos)) {
				return false;
			}

			Item item;
			if (arguments[0] == MAP_ITEM_INCREASE_RANGE) {
				item = Item::create(Item::INCREASE_RANGE, pos);
			}
			else if (arguments[0] == MAP_ITEM_EXTRA_BOMB) {
				item = Item::create(Item::EXTRA_BOMB, pos);
			}
			else if (arguments[0] == MAP_ITEM_REMOTE) {
				item = Item::create(Item::REMOTE, pos);
			}
			else {
				return false;
			}

			builder->_items.push_back(item);
		}
		else if (command == MAP_CMD_DOOR) {
			if (!buildCoordinate(arguments[0], arguments[1], builder->doorPosition)) {
				return false;
			}
			PrologHelper::GetInstance()->AddClosedDoor(builder->doorPosition.i, builder->doorPosition.j);
		}
		else {
			return false;
		}

		return true;
	}
}
