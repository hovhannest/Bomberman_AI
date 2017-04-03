#pragma once

#include <initializer_list>

namespace Bomberman {
	enum class Signal {
		ShowConsole,
		InGame,
		PauseGame,
		EndGame,
		HideConsole,
		ShowTutorial,
		LevelList,
		MainMenu,
		PlayerDead,
		PlayerAlive,
		MapEditor
	};

	bool signalIn(Signal signal, std::initializer_list<Signal> signals);
}
