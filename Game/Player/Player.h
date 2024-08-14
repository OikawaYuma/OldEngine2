#pragma once
#include <list>

#include "Object3d.h"
#include "WorldTransform.h"
#include "Camera.h"
#include "Sprite.h"
#include "IBullet.h"
#include "PlayerBullet/PlayerBullet.h"
class Player: public Collider
{
public:
	void Init(const Vector3& translate, const std::string filename);
	void Update();
	void Draw(Camera* camera);

	/// <summary>
	/// UI描画
	/// </summary>	
	void DrawUI();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	void Jump();

	void Aim();
public:// Getter
	Vector3 GetReticleWorldPosition();
	// 弾リストを取得
	const std::list<PlayerBullet*>& Getbullet() const { return bullets_; }
	// Scaleを取得
	Vector3 GetScale() { return worldTransform_.scale_; }
	// Hpを取得
	float GetHP() { return hp_; }

public: // Setter
	void SetCamera(Camera* camera) { camera_ = camera; };
	//void SetParent(Matrix4x4 cameraMatWorld) { worldTransform_.parent_ = cameraMatWorld; }
	// 主に他クラスでプレイヤーと当たり、サイズが大小するときに使う
	void SetScale(const Vector3& nextScale) { worldTransform_.scale_ = nextScale; }

	void SetHP(float hp) { hp_ = hp; }

public: // Collision

	// 衝突を検出したらコールバック関数
	void OnCollision(uint32_t attri)override;
	Vector3 GetWorldPosition() const override;
	
private:
	std::unique_ptr<Object3d> object_ = nullptr;
	std::unique_ptr<Object3d> object1_ = nullptr;
	std::unique_ptr<Object3d> object2_ = nullptr;
	std::unique_ptr<Sprite> reticleNear_ = nullptr;
	std::unique_ptr<Sprite> reticleFar_ = nullptr;
	std::unique_ptr<Sprite> hpUI_ = nullptr;
	std::unique_ptr<Sprite> hpUIBlue_ = nullptr;
	std::unique_ptr<Sprite> bulletModeUI = nullptr;

	std::list<PlayerBullet*> bullets_;
	uint32_t floorTex_;
	uint32_t playerReticleTex_;
	uint32_t playerHpUITex_;

	//bulletMode
	uint32_t bulletMode_ = Normal;
	
	// BulletModeUI
	uint32_t normalBulletUITex_;
	uint32_t hommingBulletUITex_;
	uint32_t razerBulletUITex_;

	WorldTransform worldTransform_;
	Camera* camera_ = nullptr;
	bool isJump_ = false;
	//float accel_ = 0.8f;
	// ジャンプ力
	float jumpPower = 1.0f;
	// ジャンプ力の加速的変化
	float accel_ = 0.1f;

	uint32_t jumpAgainTimer_ = 0;

	float hp_ = 1;
	Vector3 cameraToPlayerDistance_{0.0f, 7.0f, -30.0f};

	// 3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticleNear_;
	// 3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticleFar_;
};

