
#include "Color.h"
#include <SDL.h>

#include <cinttypes>
#include <sstream>

struct SDL_Color;

namespace Bomberman {
	SDL_Color Color::sdlColor() const {
		SDL_Color color;

		color.r = r;
		color.b = b;
		color.g = g;
		color.a = a;

		return color;
	}

	std::string Color::toString() const {
		std::stringstream _string;

		_string << "[" << (int)r << "," << (int)g << "," << (int)b << "," << (int)a << "]";

		return _string.str();
	}

	const uint8_t Color::OPAQUE = 0xFF;
	const uint8_t Color::TRANSPARENT = 0x00;

	const Color Color::WHITE = Color(0xFF);
	const Color Color::BLACK = Color(0x00);

	const Color Color::RED = Color(0xFF, 0x00, 0x00);
	const Color Color::GREEN = Color(0x00, 0xFF, 0x00);
	const Color Color::BLUE = Color(0x00, 0x00, 0xFF);

	const Color Color::CYAN = Color(0x00, 0xFF, 0xFF);
	const Color Color::MAGENTA = Color(0xFF, 0x00, 0xFF);
	const Color Color::YELLOW = Color(0xFF, 0xFF, 0x00);
}
