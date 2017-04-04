
#include "Player.h"
#include "Item.h"
#include "../../Core/Log/Log.h"
#include "../../Core/Log/LogLevel.h"

namespace Bomberman {
	const int Player::invincibleTime = 750;
	const int Player::defaultLifeCount = 3;

	Player::Player() : Player(defaultLifeCount) {

	}

	Player::Player(int lifes) : explosionSize(1), _maxBombs(1), dead(false), _isInvincible(false), _hasRemote(false), lifes(lifes), maxLifes(lifes) {

	}

	Coordinate& Player::position() {
		return _position;
	}

	void Player::update() {
		if (invincible.isCounting() && invincible.getTime() >= invincibleTime) {
			invincible.stop();
		}
	}

	void Player::addItem(Item item) {
		if (item == Item::EXTRA_BOMB) {
			++_maxBombs;
		}
		else if (item == Item::INCREASE_RANGE) {
			++explosionSize;
		}
		else if (item == Item::REMOTE) {
			_hasRemote = true;
		}
		else {
			Log::get() << "Player tried to process invalid item with id: " << item.id() << LogLevel::error;
		}
	}

	void Player::setInvincibility(bool invincible) {
		_isInvincible = invincible;
	}

	void Player::setRemote(bool remote) {
		_hasRemote = remote;
	}

	int Player::getExplosionSize() const {
		return explosionSize;
	}

	int Player::maxBombs() const {
		return _maxBombs;
	}

	bool Player::isDead() const {
		return dead;
	}

	bool Player::isInvincible() const {
		return _isInvincible || invincible.isCounting();
	}

	bool Player::hasRemote() const {
		return _hasRemote;
	}

	int Player::getLifesCount() const {
		return lifes;
	}

	int Player::getMaxLifes() const {
		return maxLifes;
	}

	void Player::die() {
		dead = true;
		--lifes;

		_hasRemote = false;
		_maxBombs = 1;
		explosionSize = 1;

		Log::get() << "Player lifes: " << lifes << LogLevel::info;
	}

	void Player::respawn() {
		dead = false;

		invincible.restart();
	}
}
