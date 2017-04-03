// 
// #include "PlayerCommand.h"
// #include "..\Constants.h"
// #include "..\..\Core\Utils\StringUtils.h"
// #include "..\Elements\Item.h"
// #include "..\..\Core\Log\Log.h"
// #include "..\..\Core\Log\LogLevel.h"
// #include "..\..\Core\Utils\Exception.h"
// #include "..\Map\TileMap.hpp"
// #include "..\Elements\Player.h"
// #include "..\Elements\Bomb.h"
// 
// 
// using namespace std;
// using namespace Bomberman::Constants;
// 
// namespace Bomberman {
// 	PlayerCommand::PlayerCommand(shared_ptr<Player> player, shared_ptr<TileMap> tileMap, string command, vector<string> arguments) : player(player), tileMap(tileMap), command(command), arguments(arguments) {
// 
// 	}
// 
// 	void PlayerCommand::execute() {
// 		if (command == MSG_MOVE) {
// 			move();
// 		}
// 		else if (command == MSG_IGNITE) {
// 			tileMap->explodeBomb();
// 		}
// 		else if (command == MSG_SETBOMB) {
// 			setBomb();
// 		}
// 		else if (command == MSG_EXTRA_BOMB) {
// 			if (arguments.empty()) {
// 				arguments.push_back("1");
// 			}
// 
// 			int ammount;
// 			if (arguments.size() == 1 && StringUtils::tryParseInt(arguments[0], ammount)) {
// 				for (int n = 0; n < ammount; ++n) {
// 					player->addItem(Item::EXTRA_BOMB);
// 				}
// 			}
// 			else {
// 				Log::get() << "Command \"" << OBJ_PLAYER << "." << MSG_EXTRA_BOMB << "(int) requires one optional integer argument." << LogLevel::warning;
// 			}
// 		}
// 		else if (command == MSG_INCREASE_RANGE) {
// 			player->addItem(Item::INCREASE_RANGE);
// 		}
// 		else if (command == MSG_IVINCIBLE) {
// 			if (arguments.size() == 1 && (arguments[0] == BOOL_TRUE || arguments[0] == BOOL_FALSE)) {
// 				player->setInvincibility(BOOL_TRUE == arguments[0]);
// 			}
// 			else {
// 				Log::get() << "Command \"" << OBJ_PLAYER << "." << MSG_IVINCIBLE << "(bool) requires one boolean argument." << LogLevel::error;
// 			}
// 		}
// 		else if (command == MSG_REMOTE) {
// 			if (arguments.size() == 1 && (arguments[0] == BOOL_TRUE || arguments[0] == BOOL_FALSE)) {
// 				player->setRemote(BOOL_TRUE == arguments[0]);
// 			}
// 			else {
// 				Log::get() << "Command \"" << OBJ_PLAYER << "." << MSG_REMOTE << "(bool) requires one boolean argument." << LogLevel::error;
// 			}
// 		}
// 		else {
// 			Log::get() << "Invalid command \"" << OBJ_PLAYER << "." << command << "()\"." << LogLevel::error;
// 		}
// 
// 		_finished = true;
// 	}
// 
// 	void PlayerCommand::move() {
// 		bool error = false;
// 
// 		Coordinate newPosition;
// 
// 		if (arguments.size() == 1) {
// 			string direction = arguments[0];
// 
// 			newPosition = player->position();
// 
// 			if (direction == ARG_RIGHT) {
// 				++newPosition.i;
// 			}
// 			else if (direction == ARG_UP) {
// 				--newPosition.j;
// 			}
// 			else if (direction == ARG_LEFT) {
// 				--newPosition.i;
// 			}
// 			else if (direction == ARG_DOWN) {
// 				++newPosition.j;
// 			}
// 			else {
// 				error = true;
// 			}
// 		}
// 		else if (arguments.size() == 2) {
// 			throw NotImplementedException();
// 		}
// 
// 		if (!tileMap->area().contains(newPosition)) {
// 			return;
// 		}
// 
// 		if (tileMap->tileHasBrick(newPosition) || tileMap->tileHasBomb(newPosition)) {
// 			return;
// 		}
// 
// 		player->position() = newPosition;
// 
// 		if (error) {
// 			Log::get() << "Invalid arguments for \"" << OBJ_PLAYER << "." << MSG_MOVE << "()\"" << LogLevel::error;
// 		}
// 	}
// 
// 	void PlayerCommand::setBomb() {
// 		if (!arguments.empty()) {
// 			Log::get() << "Invalid arguments for \"" << OBJ_PLAYER << "." << MSG_SETBOMB << "()\"" << LogLevel::error;
// 		}
// 
// 		if (tileMap->bombCount() < player->maxBombs() && !tileMap->tileHasBomb(player->position())) {
// 			tileMap->addBomb(Bomb(player->position()));
// 		}
// 	}
// }
