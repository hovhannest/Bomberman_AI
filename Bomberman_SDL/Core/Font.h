#pragma once

#include "Color.h"

#include <memory>
#include <string>

struct SDL_Renderer;
struct _TTF_Font;

namespace Bomberman {
	class Texture;

	class Font {
	public:
		Font();
		Font(std::string fileName, int size, std::shared_ptr<SDL_Renderer> renderer);

		Texture write(std::string string, Color color = Color::WHITE);

		std::string name() const;
		int size() const;

		void setUnderLine();

		int maxHeight();

	private:
		bool _loaded;
		std::shared_ptr<SDL_Renderer> renderer;
		std::shared_ptr<_TTF_Font> font;
		std::string _fileName;
		int _size;

		std::string fontId() const;
		std::string textureName(std::string string) const;
	};
}