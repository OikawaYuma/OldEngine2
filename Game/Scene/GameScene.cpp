#include "GameScene.h"
#include "Loder.h"
#include "ImGuiCommon.h"
void GameScene::Init()
{
	camera_ = std::make_unique<RailCamera>();
	camera_->Init();
	player_ = std::make_unique<Player>();
	player_->SetCamera(camera_->GetCamera());
	floor_ = new Floor();
	Loder::LoadJsonFile("Resources/json","stage",player_.get(),floor_,enemys_,items_,worldDesigns_);
	
	
	
	

	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->SetGameScene(this);
	collisionManager_->SetPlayer(player_.get());
	postProcess_ = new PostProcess();
	postProcess_->SetCamera(camera_->GetCamera());
	postProcess_->Init();
	IPostEffectState::SetEffectNo(PostEffectMode::kBloom);

	destroyCount_ = 0;
}

void GameScene::Update()
{
	enemys_.remove_if([=](Enemy* bullet) {
		if (bullet->IsDead()) {
			destroyCount_++;
			delete bullet;
			return true;
		}
		return false;
		});
	items_.remove_if([](Item* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
		});
	// デスフラグの立った弾を削除
	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
		});

	if (destroyCount_>=3) {
		IScene::SetSceneNo(CLEAR);
	}
	player_->Update();
	camera_->Update();
	float depthp = postProcess_->GetFarClip();
	ImGui::Begin("Depth");
	ImGui::SliderFloat("far", &depthp,0.1f,100.0f);
	ImGui::End();
	postProcess_->SerFarClip(depthp);
	floor_->Update();
	for (std::list<Enemy*>::iterator itr = enemys_.begin(); itr != enemys_.end(); itr++) {
		(*itr)->Update();
		// enemy->Fire();
		if ((*itr)->GetFireTimer() >= (*itr)->kFireInterval) {
			assert(player_);
			// 弾の速度
			const float kBulletSpeed = 1.0f;

			Vector3 start = (*itr)->GetWorldPosition();
			Vector3 end = player_->GetWorldPosition();

			Vector3 diffVector;
			diffVector.x = end.x - start.x;
			diffVector.y = end.y - start.y;
			diffVector.z = end.z - start.z;

			diffVector = Normalize(diffVector);
			diffVector.x *= kBulletSpeed;
			diffVector.y *= kBulletSpeed;
			diffVector.z *= kBulletSpeed;

			Vector3 velocity(diffVector.x, diffVector.y, diffVector.z);

			// 速度ベクトルを自機の向きに合わせて回転させる
			velocity = TransformNormal(velocity, (*itr)->GetWorldTransform().matWorld_);

			// 弾を生成し、初期化
			EnemyBullet* newBullet = new EnemyBullet();
			newBullet->Init((*itr)->GetWorldTransform().translation_, velocity);
			newBullet->SetPlayer(player_.get());
			// 弾を登録する
			enemyBullets_.push_back(newBullet);
			(*itr)->SetFireTimer(0);
		}
	}
	for (std::list<Item*>::iterator itr = items_.begin(); itr != items_.end(); itr++) {
		(*itr)->Update();
	}
	for (std::list<EnemyBullet*>::iterator itr = enemyBullets_.begin(); itr != enemyBullets_.end(); itr++) {
		(*itr)->Update();
	}

	for (std::list<WorldDesign*>::iterator itr = worldDesigns_.begin(); itr != worldDesigns_.end(); itr++) {
		(*itr)->Update();
	}

	ImGui::Begin("EnemyBullet");
	ImGui::Text("%d", enemyBullets_.size());
	ImGui::End();
	collisionManager_->CheckAllCollision();

	if (player_->GetHP()<= 0.0f) {
		IScene::SetSceneNo(GAMEOVER);
	}
	}
void GameScene::Draw()
{
	for (std::list<WorldDesign*>::iterator itr = worldDesigns_.begin(); itr != worldDesigns_.end(); itr++) {
		(*itr)->Draw(camera_->GetCamera());
	}
	floor_->Draw(camera_->GetCamera());
	
	for (std::list<Enemy*>::iterator itr = enemys_.begin(); itr != enemys_.end(); itr++) {
		(*itr)->Draw(camera_->GetCamera());
	}
	for (std::list<Item*>::iterator itr = items_.begin(); itr != items_.end(); itr++) {
		(*itr)->Draw(camera_->GetCamera());
	}
	for (std::list<EnemyBullet*>::iterator itr = enemyBullets_.begin(); itr != enemyBullets_.end(); itr++) {
		(*itr)->Draw(camera_->GetCamera());
	}
	player_->Draw(camera_->GetCamera());
	
	
}

void GameScene::Draw2d()
{
	player_->DrawUI();
}

void GameScene::PostDraw()
{
	postProcess_->Draw();
	
	
}

void GameScene::Release() {
	postProcess_->Release();
}
// ゲームを終了
int GameScene::GameClose()
{
	return false;
}

