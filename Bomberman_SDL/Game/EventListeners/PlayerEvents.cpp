
#include "PlayerEvents.h"

#include <SDL.h>

#include "../Constants.h"
#include "../../Core/Command.h"
#include "../../Core/CommandQueue.h"
#include "../../Core/EventListener.h"
#include "../CommandFactory.h"
#include "../Signal.h"
#include "../../Core/Utils/Pointer.h"
#include "../../Core/Log/Log.h"
#include "../../Core/Log/LogLevel.h"
#include "../../AI/PrologHelper.h"
#include "../Layers/GameLayer.h"
#include "../../Core/Math/Coordinate.h"
#include "../Layers/GameLayer.h"
#include <memory>

#include <iostream>

extern bool g_initAi;

using namespace std;
using namespace Bomberman::Constants;

namespace Bomberman {
	template <typename T>
	bool _lock(weak_ptr<T> in, shared_ptr<T>& out, string component) {
		bool result = lockWeakPointer(in, out);

		if (!result) {
			Log::get() << "No " << component << " for PlayerEvents" << LogLevel::error;
		}

		return result;
	}

	void PlayerEvents::listenEvent(SDL_Event event) {
		shared_ptr<Command> command;
		shared_ptr<CommandFactory> commandFactory;
		shared_ptr<CommandQueue> commandQueue;

		if (!_lock(this->commandFactory, commandFactory, "CommandFactory") ||
			!_lock(this->commandQueue, commandQueue, "CommandQueue")) {
			return;
		}

		static int bomb_set = 0;

		if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
			case SDLK_RIGHT:
				command = commandFactory->sendMessage(OBJ_PLAYER, MSG_MOVE, { ARG_RIGHT });
				break;

			case SDLK_UP:
				command = commandFactory->sendMessage(OBJ_PLAYER, MSG_MOVE, { ARG_UP });
				break;

			case SDLK_LEFT:
				command = commandFactory->sendMessage(OBJ_PLAYER, MSG_MOVE, { ARG_LEFT });
				break;

			case SDLK_DOWN:
				command = commandFactory->sendMessage(OBJ_PLAYER, MSG_MOVE, { ARG_DOWN });
				break;
			case SDLK_k:

				if (g_initAi)
				{
					int i, j;
					gameLayer->GetPleyerCoordinates(i, j);
					int walkWal = PrologHelper::GetInstance()->Walk(i, j);
					if (walkWal < 0)
					{
						if (bomb_set == 0)
						{
							command = commandFactory->sendMessage(OBJ_PLAYER, MSG_SETBOMB);
							bomb_set = 2;
						}
						walkWal = -walkWal;

						cout << "111" << endl;
					}
					if (bomb_set <= 1)
					{
						cout << "222" << endl;
						switch (walkWal)
						{
						case 1:
							command = commandFactory->sendMessage(OBJ_PLAYER, MSG_MOVE, { ARG_LEFT });
							break;
						case 2:
							command = commandFactory->sendMessage(OBJ_PLAYER, MSG_MOVE, { ARG_DOWN });
							break;
						case 3:
							command = commandFactory->sendMessage(OBJ_PLAYER, MSG_MOVE, { ARG_RIGHT });
							break;
						case 5:
							command = commandFactory->sendMessage(OBJ_PLAYER, MSG_MOVE, { ARG_UP });
							break;
						default:
							break;
						};
					}
					if (bomb_set)
					{
						bomb_set -= 1;
					}
				}
				break;

			default:
				break;
			}
		}
		else if (event.type == SDL_KEYUP) {
			switch (event.key.keysym.sym) {
			case SDLK_b:
				command = commandFactory->sendMessage(OBJ_PLAYER, MSG_SETBOMB);
				break;

			case SDLK_e:
				command = commandFactory->sendMessage(OBJ_PLAYER, MSG_IGNITE);
				break;
			case SDLK_p:
				if (g_initAi)
				{
					int i, j;
					gameLayer->GetPleyerCoordinates(i, j);
					int walkWal = PrologHelper::GetInstance()->Walk(i, j);
					if (walkWal < 0)
					{
						if (bomb_set == 0)
						{
							command = commandFactory->sendMessage(OBJ_PLAYER, MSG_SETBOMB);
							bomb_set = 2;
						}
						walkWal = -walkWal;

						cout << "111" << endl;
					}
					if(bomb_set <= 1)
					{
						cout << "222" << endl;
						switch (walkWal)
						{
						case 1:
							command = commandFactory->sendMessage(OBJ_PLAYER, MSG_MOVE, { ARG_LEFT });
							break;
						case 2:
							command = commandFactory->sendMessage(OBJ_PLAYER, MSG_MOVE, { ARG_DOWN });
							break;
						case 3:
							command = commandFactory->sendMessage(OBJ_PLAYER, MSG_MOVE, { ARG_RIGHT });
							break;
						case 5:
							command = commandFactory->sendMessage(OBJ_PLAYER, MSG_MOVE, { ARG_UP });
							break;
						default:
							break;
						};
					}
					if (bomb_set)
					{
						bomb_set -= 1;
					}
				}
				break;

			case SDLK_t:
				PrologHelper::GetInstance()->printWorld();
				break;
			case SDLK_o:
				if (g_initAi)
				{
					int i, j;
					gameLayer->GetPleyerCoordinates(i, j);
					int walkWal = PrologHelper::GetInstance()->Walk(i, j);
					if (walkWal < 0)
					{
						command = commandFactory->sendMessage(OBJ_PLAYER, MSG_SETBOMB);
						walkWal = -walkWal;
					}
				}
				break;

			default:
				break;
			}
		}

		if (command) {
			commandQueue->addCommand(command);
		}
	}

	void PlayerEvents::handleSignal(Signal signal) {
		if (signalIn(signal, { Signal::InGame, Signal::HideConsole, Signal::PlayerAlive })) {
			EventListener::enable();
		}
		else {
			EventListener::disable();
		}
	}

	void PlayerEvents::setCommandFactory(weak_ptr<CommandFactory> commandFactory) {
		this->commandFactory = commandFactory;
	}

	void PlayerEvents::setCommandQueue(weak_ptr<CommandQueue> commandQueue) {
		this->commandQueue = commandQueue;
	}
}
