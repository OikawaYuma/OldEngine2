#pragma once
#include "Object3d.h"
#include "WorldTransform.h"
#include "Camera.h"
#include "Collider.h"
class Enemy : public Collider
{

public:
	void Init(Vector3 translate);
	void Update();
	void Draw(Camera* camera);

	// 衝突を検出したらコールバック関数
	void OnCollision()override;
	bool IsDead() { return isDead_; }
	Vector3 GetWorldPosition() const override;
private:
	std::unique_ptr<Object3d> object_;
	uint32_t floorTex_;
	WorldTransform worldTransform_;

	bool isDead_ = false;
};

