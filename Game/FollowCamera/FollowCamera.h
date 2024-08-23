#pragma once
#include "Camera.h"
#include "WorldTransform.h"
#include <memory>

class FollowCamera
{
public:
	void Init();
	void Update();

public: // getter
	Camera* GetCamera() { return camera_.get(); }

	WorldTransform* GetWorldTransform() { return &worldTransform_; }
	
private:
	std::unique_ptr<Camera> camera_ = nullptr;
	WorldTransform worldTransform_;

};

