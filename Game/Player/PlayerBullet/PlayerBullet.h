#pragma once
#include "Model.h"
#include "Camera.h"
#include "Vector3.h"
#include "WorldTransform.h"
#include "Object3d.h"
#include "Collider.h"
#include "ImGuiCommon.h"

class PlayerBullet : public Collider {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(const Vector3& pos, const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(Camera* camera);
	void SetParent(const WorldTransform* parent);
	bool IsDead()const { return isDead_; }
	// 衝突を検出したらコールバック関数
	void OnCollision(uint32_t attri)override;
	Vector3 GetWorldPosition() const override;
private:
	WorldTransform worldtransform_;
	Model* model_ = nullptr;
	std::unique_ptr<Object3d> object_ = nullptr;
	uint32_t textureHandle_;
	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };
	Vector3 velocity_;

	static const int32_t kLifeTime = 60 * 5;
	DirectionalLight dir_;
	Material material;
	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;

	uint32_t texHandle_ = 0;
};
