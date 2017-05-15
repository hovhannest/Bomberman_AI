
#include "Bomb.h"
#include "..\..\AI\PrologHelper.h"

namespace Bomberman {
	const int Bomb::TTL = 3000;

	Bomb::Bomb(Coordinate position) : _position(position), _exploded(false) {
		timer.start();
	}

	Coordinate Bomb::getPosition() const {
		return _position;
	}

	void Bomb::update(bool remote, bool forceExplosion) {
		if (_exploded) {
			return;
		}

		if (remote) {
			_exploded = forceExplosion;
		}
		else if (timer.getTime() >= TTL) {
			_exploded = true;
			timer.stop();
		}
		if (_exploded)
		{
			PrologHelper::GetInstance()->Explode(getPosition().i, getPosition().j);
		}
	}

	void Bomb::forceExplosion() {
		_exploded = true;
	}

	bool Bomb::exploded() const {
		return _exploded;
	}
}