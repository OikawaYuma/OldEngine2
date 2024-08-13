#include "RailCamera.h"

void RailCamera::Init()
{
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();

	camera_->SetRotate({ 0.06f, 0.0f, 0.0f });

}

void RailCamera::Update()
{
	camera_->Update();
}
