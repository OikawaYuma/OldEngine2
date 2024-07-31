#pragma once
#include <list>

#include "Object3d.h"
#include "WorldTransform.h"
#include "Camera.h"
#include "Sprite.h"

#include "PlayerBullet/PlayerBullet.h"
class Player: public Collider
{
public:
	void Init(const Vector3& translate);
	void Update();
	void Draw(Camera* camera);

	/// <summary>
	/// UI描画
	/// </summary>	
	void DrawUI();

	void Attack();
public:// Getter
	Vector3 GetReticleWorldPosition();
	// 弾リストを取得
	const std::list<PlayerBullet*>& Getbullet() const { return bullets_; }
	Vector3 GetScale() { return worldTransform_.scale_; }

public: // Setter
	void SetCamera(Camera* camera) { camera_ = camera; };
	//void SetParent(Matrix4x4 cameraMatWorld) { worldTransform_.parent_ = cameraMatWorld; }
	// 主に他クラスでプレイヤーと当たり、サイズが大小するときに使う
	void SetScale(const Vector3& nextScale) { worldTransform_.scale_ = nextScale; }

public: // Collision

	// 衝突を検出したらコールバック関数
	void OnCollision(uint32_t attri)override;
	Vector3 GetWorldPosition() const override;
	
private:
	std::unique_ptr<Object3d> object_ = nullptr;
	std::unique_ptr<Sprite> reticle_ = nullptr;

	std::list<PlayerBullet*> bullets_;
	uint32_t floorTex_;
	uint32_t playerReticleTex_;
	WorldTransform worldTransform_;
	Camera* camera_ = nullptr;
	bool isJump_ = false;
	//float accel_ = 0.8f;
	// ジャンプ力
	float jumpPower = 1.0f;
	// ジャンプ力の加速的変化
	float accel_ = 0.1f;

	uint32_t jumpAgainTimer_ = 0;


	Vector3 cameraToPlayerDistance_{0.0f, 20.0f, -63.0f};

	// 3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;
};

