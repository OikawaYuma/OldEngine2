#include "FollowCamera.h"

void FollowCamera::Init()
{
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	camera_->SetRotate({ 0.06f, 0.0f, 0.0f });
	worldTransform_.Initialize();
	//Vector3 cameraToPlayerDistance_{ 0.0f, 7.0f, -30.0f };
	//worldTransform_.translation_ = cameraToPlayerDistance_;
}

void FollowCamera::Update()
{
	//worldTransform_.translation_.z += 0.1f;
	//worldTransform_.UpdateMatrix();
	//
	//camera_->SetTranslate({
	//	worldTransform_.matWorld_.m[3][0],
	//	worldTransform_.matWorld_.m[3][1],
	//	worldTransform_.matWorld_.m[3][2]
	//
	//	});
	camera_->Update();
}
