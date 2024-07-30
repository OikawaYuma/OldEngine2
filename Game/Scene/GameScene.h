#pragma once
#include "IScene.h"
#include "RailCamera/RailCamera.h"
#include "Player/Player.h"
#include "Floor/Floor.h"
#include "Item/Item.h"
#include "Enemy/Enemy.h"
#include "PostProcess.h"
#include "CollisionManager.h"	
#include <vector>
#include <Player/Player.h>


class GameScene :public IScene
{
public:
	void Init() override;
	void Update()override;
	void Draw()override;
	void Draw2d()override;
	void PostDraw()override;
	void Release()override;
	int GameClose()override;
public:
	// 弾リストを取得
	const std::list<Enemy*>& Getbullet() const { return enemys_; }
	const std::list<Item*>& GetItems() const { return items_; }
private:

	std::unique_ptr<RailCamera> camera_ = nullptr;
	std::unique_ptr<Player> player_ = nullptr;
	std::list<Enemy*> enemys_;
	std::list<Item*> items_;
	Floor* floor_ = nullptr;
	PostProcess* postProcess_ = nullptr;

	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;
	
};

