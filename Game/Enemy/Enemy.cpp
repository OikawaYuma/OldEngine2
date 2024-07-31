#include "Enemy.h"
#include "Player/Player.h"
void Enemy::Init(Vector3 translate)
{
	floorTex_ = TextureManager::GetInstance()->StoreTexture("Resources/Enemy.png");
	worldTransform_.Initialize();
	worldTransform_.scale_.x = 1.0f;
	worldTransform_.scale_.z = 1.0f;
	worldTransform_.translation_.x =translate.x;
	worldTransform_.translation_.y = translate.y;
	worldTransform_.translation_.z = translate.z;



	//ModelManager::GetInstance()->LoadModel("Resources/box/", "box.obj");
	object_ = std::make_unique<Object3d>();
	object_->Init();
	object_->SetModel("box.obj");
	SetCollisonAttribute(0b010);
	SetCollisionMask(0b001);

}

void Enemy::Update()
{
	object_->Update();
	object_->SetWorldTransform(worldTransform_);
	

	worldTransform_.UpdateMatrix();


}

void Enemy::Draw(Camera* camera)
{
	object_->Draw(floorTex_, camera);
}

void Enemy::OnCollision(uint32_t attri)
{

	isDead_ = true;
	if (attri == 0b0001) {
		Vector3 preScale = player_->GetScale();
		preScale = Subtract(preScale, { 0.1f,0.1f,0.1f });
		player_->SetScale(preScale);
	}
}

Vector3 Enemy::GetWorldPosition() const
{
	// ワールド行列座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
