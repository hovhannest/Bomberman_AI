#pragma once

#include <memory>

namespace Bomberman {
	template <typename T>
	bool lockWeakPointer(std::weak_ptr<T> in, std::shared_ptr<T>& out) {
		if (!in.expired()) {
			out = in.lock();
			return true;
		}
		else {
			return false;
		}
	}
}