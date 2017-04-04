#pragma once

#include <deque>
#include <SDL.h>

#include "../../Core/Drawable.h"
#include "../../Core/Updatable.h"
#include "../../Core/SignalHandler.h"
#include "../../Core/Log/Logger.h"
#include "../../Core/Timer.h"
#include "../../Core/Texture.h"
#include "../../Core/Font.h"

namespace Bomberman {
	class ConsoleLayer : public Drawable, public Updatable, public SignalHandler, public Logger {
	public:
		ConsoleLayer();

		void update();
		void draw();
		void handleSignal(Signal signal);

		void load(std::shared_ptr<SDL_Renderer> renderer);

		void screenSizeChanged(Rectangle previousSize, Rectangle newSize);

		void setInput(std::string input);
		void clearInput();

	protected:
		void recieveLog(std::string text, LogLevel level);

	private:
		static const int BLINK_TIME;
		static const int FONT_SIZE;
		static const int QUEUE_SIZE;

		Font font;
		Texture background, inputBackground;
		std::shared_ptr<SDL_Renderer> renderer;
		std::deque<Texture> lines;

		Texture prompt, inputLine, cursor;
		bool drawCursor;
		Timer cursorBlink;
	};
}
