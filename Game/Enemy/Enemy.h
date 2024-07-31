#pragma once
#include "Object3d.h"
#include "WorldTransform.h"
#include "Camera.h"
#include "Collider.h"

class Player;
class Enemy : public Collider
{

public:
	void Init(Vector3 translate);
	void Update();
	void Draw(Camera* camera);

	// 衝突を検出したらコールバック関数
	void OnCollision(uint32_t attri)override;
	bool IsDead() { return isDead_; }
	Vector3 GetWorldPosition() const override;
public: //Setter
	void SetPlayer(Player* player) { player_ = player; }

private:
	std::unique_ptr<Object3d> object_;
	uint32_t floorTex_;
	WorldTransform worldTransform_;

	Player* player_ = nullptr;
	bool isDead_ = false;
};

