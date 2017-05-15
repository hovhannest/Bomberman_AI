#pragma once

#include <vector>
#include "../Math/Random.h"

namespace Bomberman {
	namespace VectorUtils {
		template <typename T, typename F>
		void removeIf(std::vector<T>& vector, F lambda) {
			for (int n = 0; n < int(vector.size()); ++n) {
				if (lambda(vector[n])) {
					vector.erase(vector.begin() + n--);
				}
			}
		}


		template <typename T, typename F, typename C>
		void removeIf(std::vector<T>& vector, F lambda, C callback) {
			for (int n = 0; n < int(vector.size()); ++n) {
				if (lambda(vector[n])) {
					callback(vector[n]);
					vector.erase(vector.begin() + n--);
				}
			}
		}

		template <typename T>
		T& randomElement(std::vector<T>& vector) {
			return vector[Random::randomInt(static_cast<int>(vector.size()))];
		}
	}
}
