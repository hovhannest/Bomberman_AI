#pragma once

#include <list>
#include <memory>

namespace Bomberman {
	class LoopQuiter;
	class Screen;

	class MainLoop {
	public:
		MainLoop();
		~MainLoop();

		std::shared_ptr<LoopQuiter> quiter();

		void run();

		void addScreen(std::shared_ptr<Screen> screen);
		void removeScreen(std::shared_ptr<Screen> screen);

	private:
		bool hasScreen(std::shared_ptr<Screen> screen);

		std::list<std::shared_ptr<Screen>> screens;
		std::shared_ptr<LoopQuiter> _quiter;
	};
}