
#include "Random.h"

#include <cstdlib>

#define F_RAND_MAX static_cast<float>(RAND_MAX)

namespace Bomberman {
	float frand() {
		return static_cast<float>(rand());
	}

	int Random::randomInt(int max) {
		return randomInt(0, max);
	}

	int Random::randomInt(int min, int max) {
		return min + (rand() / (RAND_MAX / (max - min)));
	}

	float Random::randomFloat(float max) {
		return randomFloat(0, max);
	}

	float Random::randomFloat(float min, float max) {
		return min + (frand() / (F_RAND_MAX / (max - min)));
	}
}
