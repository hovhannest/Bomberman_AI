
#include "Texture.h"

#include <SDL.h>
#include <SDL_image.h>
#include "..\Game\Constants.h"
#include "Utils\OperatingSystem.h"
#include "Color.h"
#include "Log\Log.h"
#include "Log\LogLevel.h"

using namespace std;
using namespace Bomberman::Constants;

namespace Bomberman {
	const uint8_t Texture::OPAQUE = 255;
	const uint8_t Texture::TRANSPARENT = 0;

	Texture::Texture() : texture(nullptr), renderer(nullptr), _loaded(false), _rectangle() {

	}

	Texture::Texture(SDL_Texture *t, string name, shared_ptr<SDL_Renderer> renderer) : texture(nullptr), renderer(renderer), _loaded(false), _name(name) {
		loadTexture(t);
	}

	Texture::Texture(std::string fileName, std::shared_ptr<SDL_Renderer> renderer) : texture(nullptr), renderer(renderer), _loaded(false), _name(fileName)
	{
		string fullName = getPath({ DIR_TEXTURES }, _name);

		SDL_Texture *t = IMG_LoadTexture(renderer.get(), fullName.c_str());

		loadTexture(t);
	}

	void Texture::clear() {
		texture.reset();
		renderer.reset();
		_loaded = false;
		_rectangle = Rectangle();
		_name.clear();
	}

	void Texture::setColor(Color color) {
		if (!_loaded) {
			Log::get() << "Trying to set the color of not loaded texture \"" << _name << "\"." << LogLevel::warning;

			return;
		}

		if (SDL_SetTextureColorMod(texture.get(), color.getRed(), color.getGreen(), color.getBlue()) != 0) {
			Log::get() << "Error setting the color for texture: \"" << _name << "\": " << SDL_GetError() << LogLevel::error;
		}
	}

	void Texture::setAlpha(uint8_t alpha) {
		if (SDL_SetTextureAlphaMod(texture.get(), alpha)) {
			Log::get() << "Could not set alpha for texture: \"" << _name << "\": " << SDL_GetError() << LogLevel::warning;
		}
	}

	void Texture::draw() {
		if (!_loaded) {
			Log::get() << "Trying to draw not loaded texture \"" << _name << "\"." << LogLevel::warning;

			return;
		}

		SDL_Rect dst;

		dst.x = _rectangle.i;
		dst.y = _rectangle.j;
		dst.w = _rectangle.width;
		dst.h = _rectangle.height;

		SDL_RenderCopy(renderer.get(), texture.get(), nullptr, &dst);
	}

	bool Texture::isLoaded() const {
		return _loaded;
	}

	string Texture::name() const {
		return _name;
	}

	Coordinate& Texture::position() {
		return _rectangle;
	}

	Coordinate Texture::position() const {
		return _rectangle;
	}

	Rectangle& Texture::rectangle() {
		return _rectangle;
	}

	Rectangle Texture::rectangle() const {
		return _rectangle;
	}

	void Texture::resetSize() {
		SDL_QueryTexture(texture.get(), nullptr, nullptr, &_rectangle.width, &_rectangle.height);
	}

	void Texture::loadTexture(SDL_Texture *t) {
		if (t != nullptr) {
			texture.reset(t, SDL_DestroyTexture);
			resetSize();

			if (SDL_SetTextureBlendMode(texture.get(), SDL_BLENDMODE_BLEND)) {
				Log::get() << "Could not enable transparency for texture: \"" << _name << "\": " << SDL_GetError() << "." << LogLevel::warning;
			}

			_loaded = true;
		}
		else {
			Log::get() << "Could not load texture \"" << _name << "\"." << LogLevel::warning;
		}
	}

	Texture Texture::createRectangle(int width, int height, Color color, shared_ptr<SDL_Renderer> renderer) {
		Texture rectangle;
		stringstream name;

		name << "|" << width << "x" << height << color.toString() << "|";

		shared_ptr<SDL_Surface> surface(SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0), SDL_FreeSurface);
		if (surface) {
			if (SDL_FillRect(surface.get(), nullptr, SDL_MapRGBA(surface->format, color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha())) == 0) {
				SDL_Texture *t = SDL_CreateTextureFromSurface(renderer.get(), surface.get());
				rectangle = Texture(t, name.str(), renderer);
			}
			else {
				Log::get() << "Could not fill rectangle for texture: \"" << name.str() << "\"." << LogLevel::warning;
				Log::get() << SDL_GetError() << LogLevel::error;
			}
		}
		else {
			Log::get() << "Could not create surface \"" << name.str() << "\"." << LogLevel::warning;
		}

		return rectangle;
	}
}
