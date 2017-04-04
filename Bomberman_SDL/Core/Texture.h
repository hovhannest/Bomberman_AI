#pragma once

#include <memory>
#include <string>

#include "Math/Coordinate.h"
#include "Math/Rectangle.h"

struct SDL_Renderer;
struct SDL_Texture;

namespace Bomberman {
	class Color;

	class Texture {
		friend class Font;
	public:
		static const uint8_t OPAQUE;
		static const uint8_t TRANSPARENT;

		Texture();
		Texture(std::string fileName, std::shared_ptr<SDL_Renderer> renderer);
		static Texture createRectangle(int width, int height, Color color, std::shared_ptr<SDL_Renderer> renderer);

		void draw();

		void clear();

		void setColor(Color color);
		void setAlpha(uint8_t alpha);

		bool isLoaded() const;
		std::string name() const;
		Coordinate& position();
		Coordinate position() const;
		Rectangle& rectangle();
		Rectangle rectangle() const;
		void resetSize();


	private:
		bool _loaded;
		std::shared_ptr<SDL_Renderer> renderer;
		std::shared_ptr<SDL_Texture> texture;
		std::string _name;
		Rectangle _rectangle;

		Texture(SDL_Texture *t, std::string name, std::shared_ptr<SDL_Renderer> renderer);
		void loadTexture(SDL_Texture *t);
	};
}
