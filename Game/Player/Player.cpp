#include "Player.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Input.h"
#include "Model.h"

#include "ImGuiCommon.h"

void Player::Init(const Vector3& translate, const std::string filename)
{
	floorTex_ = TextureManager::GetInstance()->StoreTexture("Resources/player.png");
	playerReticleTex_ = TextureManager::GetInstance()->StoreTexture("Resources/Reticle.png");
	playerHpUITex_ = TextureManager::GetInstance()->StoreTexture("Resources/HpUI.png");

	hp_ = 1.0f;

	worldTransform_.Initialize();
	worldTransform_.translation_ = translate;
	worldTransform_.translation_.y = worldTransform_.scale_.y;
	// HPを元に基準となる大きさを決定する
	worldTransform_.scale_ = {hp_,hp_,hp_};
	SetRadius(hp_);

	//ModelManager::GetInstance()->LoadModel("Resources/box/", "box.obj");
	object_ = std::make_unique<Object3d>();
	object_->Init();
	object_->SetModel(filename + ".obj");

	reticle_ = std::make_unique<Sprite>();
	reticle_->Init(
		{ 720.0f ,360.0f },
		{ 100.0f, 100.0f },
		{ 0.5f , 0.5f },
		{ 1.0f, 1.0f, 1.0f, 1.0f},
		"Resources/Reticle.png");

	hpUIBlue_ = std::make_unique<Sprite>();
	hpUIBlue_->Init(
		{ hp_ * 200.0f / 2.0f +50.0f ,25.0f },
		{ hp_ * 200.0f, 50.0f },
		{ 0.5f , 0.5f },
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		"Resources/player.png");
	hpUI_ = std::make_unique<Sprite>();
	hpUI_->Init(
		{ 25.0f ,25.0f },
		{ 50.0f, 50.0f },
		{ 0.5f , 0.5f },
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		"Resources/HpUI.png");
	
	SetCollisonAttribute(0b0001);
	SetCollisionMask(0b0110);
	worldTransform3DReticle_.Initialize();
	

}

void Player::Update()
{
	// HPを元に基準となる大きさを決定する
	worldTransform_.scale_ = { hp_,hp_,hp_ };
	SetRadius(hp_);
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
		});
	Vector3 camerarotate_ = camera_->GetRotate();
	Vector3 preCameraToPlayerDistance = cameraToPlayerDistance_;
	float cameraFarY = camera_->GetFarClip();
	ImGui::Begin("Player & camera");

	ImGui::DragFloat3("playerScale", &worldTransform_.scale_.x);
	ImGui::DragFloat3("playertranlate", &worldTransform_.translation_.x);

	ImGui::DragFloat3("cameraRotate", &camerarotate_.x, 0.01f);
	ImGui::DragFloat3("cameraTranslate", &preCameraToPlayerDistance.x, 0.01f);
	ImGui::DragFloat("cameraForY", &cameraFarY, 2.5f);
	ImGui::End();
	camera_->SetRotate(camerarotate_);
	camera_->SetFarClip(cameraFarY);
	cameraToPlayerDistance_ = preCameraToPlayerDistance;

	object_->Update();
	
	Move();
	
	Jump();
	reticle_->Update();
	

	Aim();
	

	// ビューポート
	Matrix4x4 matViewport =
		MakeViewportMatrix(0, 0, WinAPI::kClientWidth_, WinAPI::kClientHeight_, 0, 1);

	/*--------合成行列の逆行列--------------*/
		// ビュープロジェクションビューポート合成行列
	Matrix4x4 matVPV =
		Multiply(Multiply(camera_->GetViewMatrix(), camera_->GetProjectionMatrix()), matViewport);
	// 合成行列の逆行列を計算する

	Matrix4x4 matInverseVPV = Inverse(matVPV);

	/*--------2点のワールド行列--------------*/
	// スクリーン座標
	Vector3 posNear = Vector3(static_cast<float>(reticle_->GetPosition().x), (float)reticle_->GetPosition().y, 0);
	Vector3 posFar = Vector3(static_cast<float>(reticle_->GetPosition().x), float(reticle_->GetPosition().y), 1);
	// スクリーン座標系からワールド座標系へ
	posNear = Transform1(posNear, matInverseVPV);
	posFar = Transform1(posFar, matInverseVPV);

	/*---------3Dレティクルの座標系さん-------*/
	// スティックレイの方向
	Vector3 spriteDierection;
	spriteDierection.x = posFar.x - posNear.x;
	spriteDierection.y = posFar.y - posNear.y;
	spriteDierection.z = posFar.z - posNear.z;
	spriteDierection = Normalize(spriteDierection);
	// カメラから照準オブジェクトの距離
	const float kDistanceTextObject = 100.0f;
	worldTransform3DReticle_.translation_.x = posNear.x + spriteDierection.x * kDistanceTextObject;
	worldTransform3DReticle_.translation_.y = posNear.y + spriteDierection.y * kDistanceTextObject;
	worldTransform3DReticle_.translation_.z = posNear.z + spriteDierection.z * kDistanceTextObject;

	worldTransform3DReticle_.UpdateMatrix();

	Attack();

	// 弾更新
	for (std::list<PlayerBullet*>::iterator itr = bullets_.begin(); itr != bullets_.end(); itr++) {
		(*itr)->Update();
	}
	
	
	worldTransform_.UpdateMatrix();
	camera_->SetTranslate({ 
		worldTransform_.translation_.x + cameraToPlayerDistance_.x,
		cameraToPlayerDistance_.y, 
		worldTransform_.translation_.z + cameraToPlayerDistance_.z });
	object_->SetWorldTransform(worldTransform_);

	hpUIBlue_->SetPosition({(hp_ * 200.0f / 2.0f) + 50.0f, 25.0f});
	hpUIBlue_->SetSize({ hp_ * 200.0f ,50.0f});
	hpUIBlue_->Update();
}

void Player::Draw(Camera* camera)
{
	object_->Draw(floorTex_,camera);
	for (std::list<PlayerBullet*>::iterator itr = bullets_.begin(); itr != bullets_.end(); itr++) {
		(*itr)->Draw(camera);
	}
}

void Player::DrawUI()
{
	reticle_->Draw(playerReticleTex_,{1.0f,1.0f,1.0f,1.0f});
	hpUI_->Draw(playerHpUITex_,{ 1.0f,1.0f,1.0f,1.0f });
	hpUIBlue_->Draw(floorTex_, { 1.0f,1.0f,1.0f,1.0f });

}

void Player::Attack()
{
	if (Input::GetInstance()->TriggerJoyButton(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
		// 自キャラの座標をコピー
		Vector3 position = {
			worldTransform_.matWorld_.m[3][0],
			worldTransform_.matWorld_.m[3][1],
			worldTransform_.matWorld_.m[3][2] };

		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
		// 自機から照準オブジェクトへのベクトル
		velocity.x = GetReticleWorldPosition().x - GetWorldPosition().x;
		velocity.y = GetReticleWorldPosition().y - GetWorldPosition().y;
		velocity.z = GetReticleWorldPosition().z - GetWorldPosition().z;



		velocity = Normalize(velocity);
		velocity.x *= kBulletSpeed;
		velocity.y *= kBulletSpeed;
		velocity.z *= kBulletSpeed;;

		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		//velocity = TransformNormal(velocity, worldTransform_.matWorld_);
		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Init(GetWorldPosition(), velocity);
		//newBullet->SetParent(worldTransform_.parent_);
		// 弾を登録する
		bullets_.push_back(newBullet);
	}

	else if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {

		// 自キャラの座標をコピー
		Vector3 position = {
			worldTransform_.matWorld_.m[3][0],
			worldTransform_.matWorld_.m[3][1],
			worldTransform_.matWorld_.m[3][2] };

		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
		// 自機から照準オブジェクトへのベクトル
		velocity.x = GetReticleWorldPosition().x - GetWorldPosition().x;
		velocity.y = GetReticleWorldPosition().y - GetWorldPosition().y;
		velocity.z = GetReticleWorldPosition().z - GetWorldPosition().z;

		

		velocity = Normalize(velocity);
		velocity.x *= kBulletSpeed;
		velocity.y *= kBulletSpeed;
		velocity.z *= kBulletSpeed;;

		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		//velocity = TransformNormal(velocity, worldTransform_.matWorld_);
		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Init(GetWorldPosition(), velocity);
		//newBullet->SetParent(worldTransform_.parent_);
		// 弾を登録する
		bullets_.push_back(newBullet);

	}
}

void Player::Move()
{

	if (Input::GetInstance()->GetJoystickState()) {
		worldTransform_.translation_.x += (float)Input::GetInstance()->GetJoyState().Gamepad.sThumbLX / SHRT_MAX * 0.4f;
	}
	else {
		if (Input::GetInstance()->PushKey(DIK_A)) {
			worldTransform_.translation_.x -= 0.5f;
		}
		if (Input::GetInstance()->PushKey(DIK_D)) {
			worldTransform_.translation_.x += 0.5f;
		}
	}

	worldTransform_.translation_.z += 0.1f;
}

void Player::Jump()
{
	// 自動ジャンプ

	/*jumpAgainTimer_++;
	if (!isJump_) {
		if (jumpAgainTimer_ >= 30) {
			jumpAgainTimer_ = 0;
			isJump_ = true;
			jumpPower = 1;
		}
	}*/
	// 手動ジャンプ


	
	if (!isJump_ && Input::GetInstance()->TriggerJoyButton(XINPUT_GAMEPAD_LEFT_SHOULDER)) {
		isJump_ = true;
		jumpPower = 1;
	} else if (!isJump_ &&Input::GetInstance()->TriggerKey(DIK_O)) {
		
		isJump_ = true;
		jumpPower = 1;
		
	}

	if (isJump_) {
		//	worldTransform_.translation_.z += 1.0f;
		worldTransform_.translation_.y += jumpPower;
		jumpPower -= accel_;
	}
	if (worldTransform_.translation_.y <= worldTransform_.scale_.y && isJump_) {
		worldTransform_.translation_.y = worldTransform_.scale_.y + 0.2f;
		jumpPower = 0.0f;
		isJump_ = false;
	}
}

void Player::Aim()
{
	// スプライトの現在座標を取得
	Vector2 spritePosition = reticle_->GetPosition();
	Input::GetInstance()->TriggerJoyButton(XINPUT_GAMEPAD_A);
	// ゲームパッド状態取得
	if (Input::GetInstance()->GetJoystickState()) {
		spritePosition.x += (float)Input::GetInstance()->GetJoyState().Gamepad.sThumbRX / SHRT_MAX * 8.0f;
		spritePosition.y -= (float)Input::GetInstance()->GetJoyState().Gamepad.sThumbRY / SHRT_MAX * 8.0f;
	}
	else {
		
		if (Input::GetInstance()->PushKey(DIK_RIGHTARROW)) {
			spritePosition.x += 5;
		}
		if (Input::GetInstance()->PushKey(DIK_LEFTARROW)) {
			spritePosition.x -= 5;
		}
		if (Input::GetInstance()->PushKey(DIK_UPARROW)) {
			spritePosition.y -= 5;
		}
		if (Input::GetInstance()->PushKey(DIK_DOWNARROW)) {
			spritePosition.y += 5;
		}
	}
	reticle_->SetPosition(spritePosition);
}

Vector3 Player::GetReticleWorldPosition()
{
	// ワールド行列座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
	worldPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
	worldPos.z = worldTransform3DReticle_.matWorld_.m[3][2];

	return worldPos;
}

void Player::OnCollision(uint32_t attri)
{
}

Vector3 Player::GetWorldPosition() const
{
	// ワールド行列座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

