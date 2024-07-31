#include "Item.h"
#include "Player/Player.h"
void Item::Init(const Vector3& translate)
{

	floorTex_ = TextureManager::GetInstance()->StoreTexture("Resources/item.png");
	worldTransform_.Initialize();
	worldTransform_.scale_.x = 1.0f;
	worldTransform_.scale_.z = 1.0f;
	worldTransform_.translation_.x = translate.x;
	worldTransform_.translation_.y = translate.y;
	worldTransform_.translation_.z = translate.z;


	//ModelManager::GetInstance()->LoadModel("Resources/box/", "box.obj");
	object_ = std::make_unique<Object3d>();
	object_->Init();
	object_->SetModel("box.obj");

	SetCollisonAttribute(0b0100);
	SetCollisionMask(0b0001);
}

void Item::Update()
{
	object_->Update();
	object_->SetWorldTransform(worldTransform_);

	worldTransform_.UpdateMatrix();
	
}

void Item::Draw(Camera* camera)
{

	object_->Draw(floorTex_, camera);
}

void Item::OnCollision()
{
	isDead_ = true;
	Vector3 preScale = player_->GetScale();
	preScale = Add(preScale, { 0.1f,0.1f,0.1f });
	player_->SetScale(preScale);

}


Vector3 Item::GetWorldPosition() const
{
	// ワールド行列座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
