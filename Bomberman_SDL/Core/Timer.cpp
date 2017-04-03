
#include "Timer.h"

#include <SDL.h>

namespace Bomberman {
	Timer::Timer() : counting(false), startTime(0), pausedTime(0), stopedTime(0), paused(false) {

	}

	void Timer::start() {
		if (!counting) {
			startTime = SDL_GetTicks();
		}

		counting = true;
		paused = false;
		pausedTime = 0;
		stopedTime = 0;
	}

	void Timer::pause() {
		if (!counting || paused) {
			return;
		}

		pausedTime = getTime();
		paused = true;
	}

	void Timer::stop() {
		stopedTime = getTime();
		counting = paused = false;
	}

	void Timer::clear() {
		counting = false;
		startTime = false;
		paused = false;

		startTime = pausedTime = stopedTime = 0;
	}

	void Timer::restart() {
		clear();
		start();
	}

	int Timer::getTime() const {
		if (paused) {
			return pausedTime;
		}
		else if (counting) {
			return SDL_GetTicks() - startTime;
		}
		else {
			return stopedTime;
		}
	}

	bool Timer::isCounting() const {
		return counting;
	}

	bool Timer::isPaused() const {
		return paused;
	}
}
