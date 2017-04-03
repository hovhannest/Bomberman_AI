
#include "Font.h"

#include <functional>
#include <SDL_ttf.h>
#include <sstream>

#include "..\Game\Constants.h"
#include "Utils\OperatingSystem.h"
#include "Log\Log.h"
#include "Log\LogLevel.h"
#include "Texture.h"

using namespace Bomberman::Constants;
using namespace std;

namespace Bomberman {
	Font::Font() : font(nullptr), renderer(nullptr), _loaded(false), _size(0) {

	}

	Font::Font(string fileName, int size, shared_ptr<SDL_Renderer> renderer) : font(nullptr), renderer(renderer), _loaded(false), _fileName(fileName), _size(size) {
		string fullName = getPath({ DIR_FONTS }, fileName);

		TTF_Font *f = TTF_OpenFont(fullName.c_str(), size);

		if (f != nullptr) {
			font.reset(f, TTF_CloseFont);

			_loaded = true;
		}
		else {
			Log::get() << "Could not load font \"" << fontId() << "\"." << LogLevel::warning;
		}
	}

	Texture Font::write(string string, Color color) {
		Texture texture;

		if (_loaded) {
			shared_ptr<SDL_Surface> surface(TTF_RenderText_Blended(font.get(), string.c_str(), color.sdlColor()), SDL_FreeSurface);

			if (surface) {
				SDL_Texture *t = SDL_CreateTextureFromSurface(renderer.get(), surface.get());

				texture = Texture(t, textureName(string), renderer);
			}
			else {
				Log::get() << "Could not create surface for text: '" << string << "' ";
				Log::get() << "with font \"" << fontId() << "\"." << LogLevel::warning;
			}
		}
		else {
			Log::get() << "Trying to draw text with unloaded font: \"" << fontId() << "\"" << LogLevel::warning;
		}

		return texture;
	}

	string Font::name() const {
		return _fileName;
	}

	int Font::size() const {
		return _size;
	}

	void Font::setUnderLine() {
		TTF_SetFontStyle(font.get(), TTF_STYLE_UNDERLINE);
	}

	int Font::maxHeight() {
		return TTF_FontHeight(font.get());
	}

	string Font::fontId() const {
		stringstream id;

		id << "#" << _fileName << "(" << _size << ")";

		return id.str();
	}

	string Font::textureName(string str) const {
		stringstream name;
		hash<string> hash_fn;

		name << fontId() << "#" << hash_fn(str);

		return name.str();
	}
}
