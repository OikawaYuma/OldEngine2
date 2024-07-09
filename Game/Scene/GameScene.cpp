#include "GameScene.h"

#include "ImGuiCommon.h"
void GameScene::Init()
{
	camera_ = std::make_unique<RailCamera>();
	camera_->Init();
	player_ = std::make_unique<Player>();
	player_->Init();
	player_->SetCamera(camera_->GetCamera());
	
	flooar_ = new Floor();
	flooar_->Init();
	
	item_ = new Item();
	item_->Init();

	enemy_ = new Enemy();
	enemy_->Init();

	postProcess_ = new PostProcess();
	postProcess_->SetCamera(camera_->GetCamera());
	postProcess_->Init();
	IPostEffectState::SetEffectNo(PostEffectMode::kBloom);
}

void GameScene::Update()
{
	
	
	player_->Update();
	camera_->Update();
	flooar_->Update();
	item_->Update();
	enemy_->Update();
	}
void GameScene::Draw()
{
	
	
	item_->Draw(camera_->GetCamera());
	enemy_->Draw(camera_->GetCamera());
	player_->Draw(camera_->GetCamera());
	flooar_->Draw(camera_->GetCamera());
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

