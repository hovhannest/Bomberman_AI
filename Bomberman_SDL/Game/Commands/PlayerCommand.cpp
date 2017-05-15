
#include "PlayerCommand.h"

#include "../../Core/Log/LogLevel.h"
#include "../../Core/Log/Log.h"
#include "../Constants.h"
#include "../Elements/Player.h"
#include "../Elements/Bomb.h"
#include "../Map/TileMap.h"
#include "../../Core/Utils/Exception.h"
#include "../Elements/Item.h"
#include "../../Core/Utils/StringUtils.h"
#include "../../AI/PrologHelper.h"

Bomberman::PlayerCommand::PlayerCommand(std::shared_ptr<Player> player, std::shared_ptr<TileMap> tileMap, std::string command, std::vector<std::string> arguments)
	: player(player), tileMap(tileMap), command(command), arguments(arguments)
{

}

void Bomberman::PlayerCommand::execute()
{
	if (command == Bomberman::Constants::MSG_MOVE) {
		move();
	}
	else if (command == Bomberman::Constants::MSG_IGNITE) {
		tileMap->explodeBomb();
	}
	else if (command == Bomberman::Constants::MSG_SETBOMB) {
		setBomb();
	}
	else if (command == Bomberman::Constants::MSG_EXTRA_BOMB) {
		if (arguments.empty()) {
			arguments.push_back("1");
		}

		int ammount;
		if (arguments.size() == 1 && StringUtils::tryParseInt(arguments[0], ammount)) {
			for (int n = 0; n < ammount; ++n) {
				player->addItem(Item::EXTRA_BOMB);
			}
		}
		else {
			Log::get() << "Command \"" << Bomberman::Constants::OBJ_PLAYER << "." << Bomberman::Constants::MSG_EXTRA_BOMB << "(int) requires one optional integer argument." << LogLevel::warning;
		}
	}
	else if (command == Bomberman::Constants::MSG_INCREASE_RANGE) {
		player->addItem(Item::INCREASE_RANGE);
	}
	else if (command == Bomberman::Constants::MSG_IVINCIBLE) {
		if (arguments.size() == 1 && (arguments[0] == Bomberman::Constants::BOOL_TRUE || arguments[0] == Bomberman::Constants::BOOL_FALSE)) {
			player->setInvincibility(Bomberman::Constants::BOOL_TRUE == arguments[0]);
		}
		else {
			Log::get() << "Command \"" << Bomberman::Constants::OBJ_PLAYER << "." << Bomberman::Constants::MSG_IVINCIBLE << "(bool) requires one boolean argument." << LogLevel::error;
		}
	}
	else if (command == Bomberman::Constants::MSG_REMOTE) {
		if (arguments.size() == 1 && (arguments[0] == Bomberman::Constants::BOOL_TRUE || arguments[0] == Bomberman::Constants::BOOL_FALSE)) {
			player->setRemote(Bomberman::Constants::BOOL_TRUE == arguments[0]);
		}
		else {
			Log::get() << "Command \"" << Bomberman::Constants::OBJ_PLAYER << "." << Bomberman::Constants::MSG_REMOTE << "(bool) requires one boolean argument." << LogLevel::error;
		}
	}
	else {
		Log::get() << "Invalid command \"" << Bomberman::Constants::OBJ_PLAYER << "." << command << "()\"." << LogLevel::error;
	}

	_finished = true;
}

void Bomberman::PlayerCommand::move()
{
	bool error = false;

	Coordinate newPosition;

	if (arguments.size() == 1) {
		std::string direction = arguments[0];

		newPosition = player->position();

		if (direction == Bomberman::Constants::ARG_RIGHT) {
			++newPosition.i;
		}
		else if (direction == Bomberman::Constants::ARG_UP) {
			--newPosition.j;
		}
		else if (direction == Bomberman::Constants::ARG_LEFT) {
			--newPosition.i;
		}
		else if (direction == Bomberman::Constants::ARG_DOWN) {
			++newPosition.j;
		}
		else {
			error = true;
		}
	}
	else if (arguments.size() == 2) {
		throw NotImplementedException();
	}

	if (!tileMap->area().contains(newPosition)) {
		return;
	}

	if (tileMap->tileHasBrick(newPosition) || tileMap->tileHasBomb(newPosition)) {
		return;
	}

	bool isNewPosition = false;
	if (player->position().i != newPosition.i || player->position().j!= newPosition.j)
	{
		isNewPosition = true;
	}
	player->position() = newPosition;

	if(isNewPosition)
		PrologHelper::GetInstance()->SetAgentLocation(newPosition.i, newPosition.j);

	if (error) {
		Log::get() << "Invalid arguments for \"" << Bomberman::Constants::OBJ_PLAYER << "." << Bomberman::Constants::MSG_MOVE << "()\"" << LogLevel::error;
	}
}

void Bomberman::PlayerCommand::setBomb()
{
	if (!arguments.empty()) {
		Log::get() << "Invalid arguments for \"" << Bomberman::Constants::OBJ_PLAYER << "." << Bomberman::Constants::MSG_SETBOMB << "()\"" << LogLevel::error;
	}

	if (tileMap->bombCount() < player->maxBombs() && !tileMap->tileHasBomb(player->position())) {
		tileMap->addBomb(Bomb(player->position()));

		PrologHelper::GetInstance()->SetBomb();
	}
}
