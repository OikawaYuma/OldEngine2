#include "GameScene.h"
#include "Loder.h"
#include "ImGuiCommon.h"
void GameScene::Init()
{
	camera_ = std::make_unique<RailCamera>();
	camera_->Init();
	player_ = std::make_unique<Player>();
	floor_ = new Floor();
	Loder::LoadJsonFile("Resources/json","stage",player_.get(),floor_);
	player_->SetCamera(camera_->GetCamera());
	
	
	
	for (int i = 0; i < 5; i++) {
		Item* item = new Item();
		item->Init({ 5.0f + i *-1.0f,1,100.0f + i * 40 });
		item->SetPlayer(player_.get());
		items_.push_back(item);
	}
	for (int i = 0; i < 5; i++) {
		Enemy* enemy = new Enemy();
		enemy->Init({ -2.0f + i,1,10.0f + i * 10 });
		enemy->SetPlayer(player_.get());
		enemys_.push_back(enemy);
	}

	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->SetGameScene(this);
	collisionManager_->SetPlayer(player_.get());
	postProcess_ = new PostProcess();
	postProcess_->SetCamera(camera_->GetCamera());
	postProcess_->Init();
	IPostEffectState::SetEffectNo(PostEffectMode::kOutline);
}

void GameScene::Update()
{
	enemys_.remove_if([](Enemy* bullet) {
		if (bullet->IsDead()) {
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
	}
	for (std::list<Item*>::iterator itr = items_.begin(); itr != items_.end(); itr++) {
		(*itr)->Update();
	}
	collisionManager_->CheckAllCollision();
	}
void GameScene::Draw()
{
	
	floor_->Draw(camera_->GetCamera());
	for (std::list<Enemy*>::iterator itr = enemys_.begin(); itr != enemys_.end(); itr++) {
		(*itr)->Draw(camera_->GetCamera());
	}
	for (std::list<Item*>::iterator itr = items_.begin(); itr != items_.end(); itr++) {
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

