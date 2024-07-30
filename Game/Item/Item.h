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

public:
	// 衝突を検出したらコールバック関数
	void OnCollision()override;

public: // Getter

	Vector3 GetWorldPosition() const override;

public: // Setter
	void SetPllayer(Player* player) { player_ = player; }

private:
	Player* player_ = nullptr;
	std::unique_ptr<Object3d> object_;

	uint32_t floorTex_;
	WorldTransform worldTransform_;


};
