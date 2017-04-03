#pragma once

namespace Bomberman {
	class Command {
	public:
		Command() : _finished(false) {}

		virtual ~Command() {};

		virtual void execute() = 0;

		inline bool finished() const {
			return _finished;
		}

	protected:
		bool _finished;
	};
}