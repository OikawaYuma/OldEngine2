#pragma once
#include "IScene.h"
#include "FollowCamera/FollowCamera.h"
#include "Player/Player.h"
#include "Floor/Floor.h"
#include "Item/Item.h"
#include "Enemy/Enemy.h"
#include "PostProcess.h"
#include "CollisionManager.h"	
#include <vector>
#include "Enemy/EnemyBullet/EnemyBullet.h"
#include "WorldDesign/WorldDesign.h"
#include "LockOn/LockOn.h"



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
	const std::list<EnemyBullet*>& GetEnemyBullets() const { return enemyBullets_; }

private:


	std::unique_ptr<FollowCamera> camera_ = nullptr;
	std::unique_ptr<Player> player_ = nullptr;
	std::list<Enemy*> enemys_;
	std::list<EnemyBullet*> enemyBullets_;
	std::list<Item*> items_;
	std::list<WorldDesign*> worldDesigns_;
	Floor* floor_ = nullptr;
	std::unique_ptr<LockOn> lockOn_;

	PostProcess* postProcess_ = nullptr;
	uint32_t destroyCount_ = 0;
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;
	
};

