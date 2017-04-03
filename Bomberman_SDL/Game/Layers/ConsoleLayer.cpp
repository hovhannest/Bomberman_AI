
#include "ConsoleLayer.h"
#include "..\Signal.h"
#include "..\..\Core\Log\LogLevel.h"


using namespace std;

namespace Bomberman {
	const int ConsoleLayer::BLINK_TIME = 500;
	const int ConsoleLayer::FONT_SIZE = 8;
	const int ConsoleLayer::QUEUE_SIZE = 15;

	ConsoleLayer::ConsoleLayer() : drawCursor(true) {
		Drawable::disable();
		cursorBlink.start();
	}

	void ConsoleLayer::update() {
		if (cursorBlink.getTime() >= BLINK_TIME) {
			cursorBlink.clear();
			cursorBlink.start();

			drawCursor = !drawCursor;
		}
	}

	void ConsoleLayer::draw() {
		background.draw();
		inputBackground.draw();

		// long because warnings and stuff
		auto lines = this->lines;
		int jPrev = prompt.rectangle().j;
		for (int n = 0; n < int(lines.size()); ++n) {
			Texture& line = lines[n];

			line.rectangle().i = prompt.rectangle().i;
			line.rectangle().j = jPrev - line.rectangle().height;

			line.draw();

			jPrev = line.rectangle().j;
		}

		prompt.draw();

		if (inputLine.isLoaded()) {
			inputLine.draw();

			cursor.rectangle().i = inputLine.rectangle().right();
		}
		else {
			cursor.rectangle().i = prompt.rectangle().right();
		}

		if (drawCursor) {
			cursor.draw();
		}
	}

	void ConsoleLayer::handleSignal(Signal signal) {
		if (Signal::ShowConsole == signal) {
			Drawable::enable();
			Updatable::enable();
		}
		else {
			Drawable::disable();
			Updatable::disable();
		}
	}

	void ConsoleLayer::load(shared_ptr<SDL_Renderer> renderer) {
		this->renderer = renderer;
		font = Font("PressStart2P.ttf", FONT_SIZE, renderer);

		prompt = font.write("> ");
		cursor = font.write("_");
	}

	void ConsoleLayer::screenSizeChanged(Rectangle previousSize, Rectangle newSize) {
		// Generate the background
		background = Texture::createRectangle(newSize.width, (int)(newSize.height * .2), Color(0x55, 0x55, 0x55), renderer);

		// Set prompt position relative to the background
		prompt.rectangle().i = background.rectangle().i;
		prompt.rectangle().j = background.rectangle().bottom() - prompt.rectangle().height;

		// Generate the background for the input
		inputBackground = Texture::createRectangle(newSize.width, prompt.rectangle().height, Color(0x30), renderer);
		inputBackground.rectangle().i = prompt.rectangle().i;
		inputBackground.rectangle().j = prompt.rectangle().j;

		// Set cursor position relative to the prompt
		cursor.position().i = prompt.rectangle().right();
		cursor.position().j = prompt.rectangle().j;
	}

	void ConsoleLayer::setInput(string input) {
		inputLine = font.write(input, Color::WHITE);

		inputLine.rectangle().i = prompt.rectangle().right();
		inputLine.rectangle().j = prompt.rectangle().j;
	}

	void ConsoleLayer::clearInput() {
		inputLine.clear();
	}

	void ConsoleLayer::recieveLog(string text, LogLevel level) {
		if (lines.size() > QUEUE_SIZE) {
			lines.pop_back();
		}

		Color color = Color::BLACK;

		if (LogLevel::info == level) {
			color = Color::GREEN;
		}
		else if (LogLevel::warning == level) {
			color = Color::YELLOW;
		}
		else if (LogLevel::error == level || LogLevel::fatal == level) {
			color = Color::RED;
		}

		Texture line = font.write(text, color);
		lines.push_front(line);
	}
}
