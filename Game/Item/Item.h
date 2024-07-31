#pragma once
#include "Object3d.h"
#include "WorldTransform.h"
#include "Camera.h"
#include "Collider.h"

class Player;
class Item : public Collider
{

public:
	void Init(const Vector3& translate);
	void Update();
	void Draw(Camera* camera);

	bool IsDead() { return isDead_; }

public:
	// 衝突を検出したらコールバック関数
	void OnCollision()override;

public: // Getter

	Vector3 GetWorldPosition() const override;

public: // Setter
	void SetPlayer(Player* player) { player_ = player; }

private:
	Player* player_ = nullptr;
	std::unique_ptr<Object3d> object_;

	bool isDead_ = false;
	uint32_t floorTex_;
	WorldTransform worldTransform_;


};
