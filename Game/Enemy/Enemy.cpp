#include "Enemy.h"
#include "Player/Player.h"
void Enemy::Init(Vector3 translate, const std::string filename)
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
	object_->SetModel( "box.obj");
	SetCollisonAttribute(0b010);
	SetCollisionMask(0b001);

}

void Enemy::Update()
{
	FireTimer++;
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
		float playerHP = player_->GetHP();
		playerHP -= 0.1f;
		player_->SetHP(playerHP);

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

void Enemy::FazeInit()
{
	FireLoop();
}

void Enemy::Fire()
{
	// 自キャラの座標をコピー
	Vector3 position = worldTransform_.translation_;

	// 弾の速度
	const float kBulletSpeed = -1.0f;
	Vector3 velocity(0, kBulletSpeed, kBulletSpeed);

	velocity = TransformNormal(velocity, worldTransform_.matWorld_);
	// 弾を生成し、初期化
	//EnemyBullet* newBullet = new EnemyBullet();
	//newBullet->Init(worldTransform_.translation_, velocity);
	//newBullet->SetPlayer(player_);

	// 弾を登録する
	//ullets_.push_back(newBullet);
}

void Enemy::FireLoop()
{
	Fire();
	// 発車タイマーをリセットする
	std::function<void(void)> callback = std::bind(&Enemy::FireLoop, this);

	// 時限発動イベントを生成
	TimedCall* timedCall = new TimedCall(callback, kFireInterval);

	timedCalls_.push_back(timedCall);
}

void Enemy::FireCount()
{
	FireTimer--;
}

void Enemy::SetVelo(Vector3 velocity)
{
	velocity_.x = velocity.x;
	velocity_.y = velocity.y;
	velocity_.z = velocity.z;
}

void Enemy::SetFireTimer(int fireTimer)
{
	FireTimer = fireTimer;
}
