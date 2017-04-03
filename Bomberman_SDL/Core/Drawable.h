#pragma once

#include <memory>
#include "ScreenComponent.h"

namespace Bomberman {
	class Rectangle;

	class Drawable : public ScreenComponent {
	public:
		virtual void draw() = 0;

		virtual void screenSizeChanged(Rectangle previousSize, Rectangle newSize) {}
	};
}