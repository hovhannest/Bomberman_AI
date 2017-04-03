#pragma once

namespace Bomberman {
	class Timer {
	public:
		Timer();

		void start();
		void pause();
		void stop();
		void clear();
		void restart();

		int getTime() const;
		bool isCounting() const;
		bool isPaused() const;
	private:
		bool counting, paused;
		int startTime, pausedTime, stopedTime;
	};
}