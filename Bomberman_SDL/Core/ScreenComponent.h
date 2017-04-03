#pragma once

namespace Bomberman {
	class ScreenComponent {
	public:
		ScreenComponent();
		virtual ~ScreenComponent();

		bool isEnabled() const;
		void enable();
		void disable();

		bool isFinished() const;
		void finish();

	private:
		bool enabled;
		bool finished;
	};
}