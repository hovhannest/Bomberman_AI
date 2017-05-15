#pragma once

#include "..\Core\Utils\Singleton.h"

#include "SWI-cpp.h"
#include "SWI-Prolog.h"
#include "..\Game\Layers\GameLayer.h"

class PrologHelper: public Singleton<PrologHelper>
{
public:
	void Init(int w, int h);
	void AgentLocation(int x, int y);

	bool AddUndestructableWall(int i, int j);
	bool AddDestructableWall(int i, int j);
	void AddClosedDoor(int i, int j);

	void SetBomb();

	int Walk(int i, int j);

	void SetAgentLocation(int i, int j);

	std::shared_ptr<Bomberman::GameLayer> GameLayer() const { return gameLayer; }
	void GameLayer(std::shared_ptr<Bomberman::GameLayer> val) { gameLayer = val; }

	void Explode(int i, int j);

	void DeleteWall(int i, int j);

	void printWorld();
private:
	std::shared_ptr<Bomberman::GameLayer> gameLayer;
};

