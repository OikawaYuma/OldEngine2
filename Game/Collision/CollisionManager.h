#pragma once
#include "Collider.h"
#include<list>
#include <memory>
class GameScene;
class Player;
class Enemy;
class CollisionManager {
public:
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);
	void CheckAllCollision();
	// ClliderにPushする
	void PushClider(Collider* collider);
	void SetGameScene(GameScene* gamescene) { gameScene_ = gamescene; }
	void SetPlayer(std::unique_ptr<Player> player) { player_ = player.get(); }
	void SetEnemy(Enemy* enemy) { enemy_ = enemy; }
private:
	GameScene* gameScene_ = nullptr;
	std::list<Collider*> colliders_;
	// 自キャラ
	Player* player_ = nullptr;

	// 敵キャラ
	Enemy* enemy_ = nullptr;
};

