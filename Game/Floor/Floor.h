#pragma once
#include "Object3d.h"
#include "WorldTransform.h"
#include "Camera.h"
class Floor
{

public:
	void Init(const Vector3& scale,const Vector3& translate);
	void Update();
	void Draw(Camera* camera);

private:
	std::unique_ptr<Object3d> object_;
	uint32_t floorTex_;
	WorldTransform worldTransform_;
	Camera* camera_ = nullptr;
};


