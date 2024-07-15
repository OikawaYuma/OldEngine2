#pragma once
#include "IScene.h"
#include "RailCamera/RailCamera.h"
#include "Player/Player.h"
#include "Floor/Floor.h"
#include "Item/Item.h"
#include "Enemy/Enemy.h"
#include "PostProcess.h"

#include <vector>
#include <Player.h>


class GameScene :public IScene
{
public:
	void Init() override;
	void Update()override;
	void Draw()override;
	void PostDraw()override;
	void Release()override;
	int GameClose()override;
private:
	std::unique_ptr<RailCamera> camera_ = nullptr;
	std::unique_ptr<Player> player_ = nullptr;
	Floor* flooar_ = nullptr;
	Item* item_ = nullptr;
	Enemy* enemy_ = nullptr;
	PostProcess* postProcess_ = nullptr;
	
};

