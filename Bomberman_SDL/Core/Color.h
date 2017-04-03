#pragma once


#include <cinttypes>
#include <string>

struct SDL_Color;

namespace Bomberman {
	class Color {
	public:
		Color(uint8_t value = 0) {}
		Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = OPAQUE) : r(r), g(g), b(b), a(a) {}

		uint8_t getRed() const {
			return r;
		}
		uint8_t getGreen() const {
			return g;
		}
		uint8_t getBlue() const {
			return b;
		}
		uint8_t getAlpha() const {
			return a;
		}

		void setRed(uint8_t r) {
			this->r = r;
		}
		void setGreen(uint8_t g) {
			this->g = g;
		}
		void setBlue(uint8_t b) {
			this->b = b;
		}
		void setAlpha(uint8_t a) {
			this->a = a;
		}

		SDL_Color sdlColor() const;

		std::string toString() const;

		static const uint8_t OPAQUE;
		static const uint8_t TRANSPARENT;

		static const Color WHITE;
		static const Color BLACK;

		static const Color RED;
		static const Color GREEN;
		static const Color BLUE;

		static const Color CYAN;
		static const Color MAGENTA;
		static const Color YELLOW;

	private:
		uint8_t r, g, b, a;
	};
}