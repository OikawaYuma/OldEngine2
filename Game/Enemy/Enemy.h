#pragma once
#include "Object3d.h"
#include "WorldTransform.h"
#include "Camera.h"
#include "Collider.h"
#include <TimedCall.h>

class Player;
class Enemy : public Collider
{

public:
	void Init(Vector3 translate, const std::string filename);
	void Update();
	void Draw(Camera* camera);

	// 衝突を検出したらコールバック関数
	void OnCollision(uint32_t attri)override;
	bool IsDead() { return isDead_; }
	Vector3 GetWorldPosition() const override;
	void FazeInit();
	void Fire();
	void FireLoop();
	void FireCount();

	const static int kFireInterval = 60;
public: // Getter
	int GetFireTimer() { return FireTimer; };
	int GetRadius() { return radius_; }
	WorldTransform GetWorldTransform() { return worldTransform_; }
public: //Setter
	void SetVelo(Vector3 velocity);
	void SetFireTimer(int fireTimer);
	void SetPlayer(Player* player) { player_ = player; }

private:
	std::unique_ptr<Object3d> object_;
	uint32_t floorTex_;
	WorldTransform worldTransform_;
	Vector3 velocity_;

	Player* player_ = nullptr;
	bool isDead_ = false;
	int radius_ = 1;
	int32_t FireTimer = 0;
	// 弾
	//std::list<EnemyBullet*> bullets_;
	std::function<void(void)> fireLoop;
	// 時限発動のリスト
	std::list<TimedCall*> timedCalls_;
};

