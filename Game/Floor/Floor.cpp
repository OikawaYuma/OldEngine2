#include "Floor.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Model.h"
void Floor::Init(const Vector3& scale, const Vector3& translate)
{
	floorTex_ = TextureManager::GetInstance()->StoreTexture("Resources/floor/floor.png");
	worldTransform_.Initialize();
	worldTransform_.scale_ = scale;
	worldTransform_.translation_ = translate;

	ModelManager::GetInstance()->LoadModel("Resources/box/", "box.obj");
	object_ = std::make_unique<Object3d>();
	object_->Init();
	object_->SetModel("box.obj");




}

void Floor::Update()
{
	object_->Update();

	worldTransform_.UpdateMatrix();
	object_->SetWorldTransform(worldTransform_);
}

void Floor::Draw(Camera* camera)
{
	camera_ = camera;
	object_->Draw(floorTex_, camera_);
}

